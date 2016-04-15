#include "ReqServer.h"


#include <iostream>
#include <string>
#include <sstream>

#include <SimpleJSON.h>
#include <logger.h>

/**************************************************************
 *                Utilities
 **************************************************************/

namespace {
    NewIntField(errorNumber);
    NewStringField(errorText);
    typedef SimpleParsedJSON<errorNumber,errorText> ErrorMsg;

    std::string ErrorMessage(const std::string& msg) {
        std::string response;
        static ErrorMsg errJSON;
        errJSON.Clear();

        errJSON.Get<errorNumber>() = 0;
        errJSON.Get<errorText>() = msg;
        response = "ERROR ";
        response += errJSON.GetJSONString();
        return response;
    }

}

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(RequestServer* s, server* raw_server, websocketpp::connection_hdl hdl, message_ptr msg) {
    if (msg->get_payload() == "stop-listening") {
        raw_server->stop_listening();
    } else {
        SLOG_FROM(LOG_VERBOSE,">> ReqServer::on_message",
        "Received message from: " << hdl.lock().get() << endl <<
        "Message: " << msg->get_payload() << endl);
        const std::string& reply = s->HandleMessage(msg->get_payload(),raw_server,hdl);

        if (reply != "") {
            SLOG_FROM(LOG_VERBOSE,"<< ReqServer::on_message",
            "Sending Message to:" << hdl.lock().get() << endl <<
            "Message: " << reply << endl);
            raw_server->send(hdl,reply,websocketpp::frame::opcode::TEXT);
        }
    }
}

void on_close(RequestServer*s, websocketpp::connection_hdl hdl) {
    SLOG_FROM(LOG_VERBOSE,"ReqServer::on_close",
    "Closing session to: " << hdl.lock().get() << endl);
    s->HandleClose(hdl);
}

/**************************************************************
 *                Data Subscriptions
 **************************************************************/
class Request: public SubscriptionHandler::SubRequest {
public:
    Request(
        const std::string& req,
        server* s,
        websocketpp::connection_hdl c)
    : request(req), serv(s), conn(serv->get_con_from_hdl(c)), open(true)
    {
    }

    virtual ~Request() { };

    const char* RequestMessasge() {
        return request.c_str();
    }

    /**
     * Send a data update, a JSON message, down the pipe
     */
    void SendMessage(const std::string& msg) {
        SLOG_FROM(LOG_VERBOSE,"<< Request::SendMessage",
        "Sending Message to:" << conn.get() << endl <<
        "Message: " << msg << endl);
        serv->send(conn,msg,websocketpp::frame::opcode::TEXT);
    }

    bool Ok() const { return open; }

    void Close () {
        open = false;
    }

private:
    std::string                 request;
    server*                     serv;
    server::connection_ptr      conn;
    bool                        open;
};

/**************************************************************
 *                Request Serve
 **************************************************************/

RequestServer::RequestServer() {
    // Set logging settings
    echo_server.set_access_channels(websocketpp::log::alevel::all);
    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    echo_server.init_asio();
}

void RequestServer::AddHandler(
         const std::string& requestName, 
         std::unique_ptr<RequestReplyHandler>&& handler) 
{
    req_handlers[requestName].reset(handler.release());
}

void RequestServer::AddHandler(
         const std::string& requestName, 
         std::unique_ptr<SubscriptionHandler>&& handler)
{
    sub_handlers[requestName].reset(handler.release());
}

std::string RequestServer::HandleMessage(
    const std::string& request,
    server* raw_server,
    websocketpp::connection_hdl hdl)
{

    std::string response = "";
    std::stringstream strRequest(request);
    std::string reqName;
    strRequest >> reqName;

    auto req_it = req_handlers.find(reqName);

    if ( req_it != req_handlers.end()) {
        response = HandleRequestReplyMessage(reqName,request,*(req_it->second));
    } else {
        auto sub_it = sub_handlers.find(reqName);
        if ( sub_it != sub_handlers.end()) {
            response = 
                HandleSubscriptionMessage(reqName, request, *(sub_it->second),raw_server,hdl);
        } else {
            response = ErrorMessage("No such request: " + reqName);
        }
    }
    return response;
}

std::string RequestServer::HandleRequestReplyMessage(
                    const std::string& reqName,
                    const std::string& request,
                    RequestReplyHandler& handler) 
{
    std::string response = "";
    // "REQUEST_NAME { name: JSON_REQUEST, ...}"
    unsigned int offset = reqName.length() + 1;
    const char* jsonRequest = "";
    if (request.length() > offset) {
        jsonRequest = request.c_str() + offset;
    }
    try {
        response = handler.OnRequest(jsonRequest);
    } catch (const RequestReplyHandler::InvalidRequestException& e) {
        response = ErrorMessage(e.errMsg);
    }

    return response;
}

void RequestServer::HandleClose(websocketpp::connection_hdl hdl) {
    void* conn = hdl.lock().get();

    auto it = conn_map.find(conn);

    if (it != conn_map.end()) {
        it->second->Close();
        conn_map.erase(it);
    }

}

std::string RequestServer::HandleSubscriptionMessage(
                    const std::string& reqName,
                    const std::string& request,
                    SubscriptionHandler& handler,
                    server*  raw_server,
                    websocketpp::connection_hdl hdl)
{
    std::string response = "";
    // "REQUEST_NAME { name: JSON_REQUEST, ...}"
    unsigned int offset = reqName.length() + 1;
    const char* jsonRequest = "";
    if (request.length() > offset) {
        jsonRequest = request.c_str() + offset;
    }
    try {
        SubscriptionHandler::RequestHandle reqHdl(
            new Request(jsonRequest,raw_server,hdl));
        handler.OnRequest(reqHdl);
    } catch (const SubscriptionHandler::InvalidRequestException& e) {
        response = ErrorMessage(e.errMsg);
    }

    return response;
}


void RequestServer::HandleRequests(unsigned short port) {
    try {
        echo_server.set_message_handler(bind(&on_message,this,&echo_server,::_1,::_2));
        echo_server.set_close_handler(bind(&on_close,this,::_1));

        echo_server.listen(port);

        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << "******************************************************" << std::endl;
        std::cout << "Web Sockets ERROR: " << e.what() << std::endl;
        std::cout << "******************************************************" << std::endl;
    }
}

void RequestServer::PostTask(const RequestServer::InteruptHandler& f) {
    boost::asio::io_service& serv = echo_server.get_io_service();
    
    serv.post(f);
}
