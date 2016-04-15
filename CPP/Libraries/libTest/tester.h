#ifndef TESTER_H
#define TESTER_H
#include <functional>
#include <string>
#include <sstream>
#include "logger.h"
#include "util_time.h"
#include <fstream>
#include <queue>


using namespace std;

class dummyLog
{
public:
     // Dummy constructor
     dummyLog() {}
     dummyLog(const basic_stringstream<char>&rhs) {}

     template<class T>
     inline dummyLog& operator<<(const T& a) { return *this;}
     template<class T>
     inline dummyLog& operator<<(const char *) { return *this;}
     inline string str() const { return ""; }

     // Handle endl
     static dummyLog& endl(dummyLog& stream)
     {
         return stream;
     }

     // Handle endl
     static dummyLog endl(basic_stringstream<char>& stream)
     {
         return stream;
     }

    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

    // this is the function signature of std::endl
    typedef CoutType& (*StandardManip)(CoutType&);

    // define an operator<< to take in std::endl
    dummyLog& operator<<(StandardManip manip)
    {
        // Drop the manipulation, and just return the stream
        return *this;
    }

    void WriteLog(const string& descr, const Time& t) {
    }

    void ReportStringDiff(const string& expected, const string actual) {
    }

    void FlushQueue() { }
}; 
class DefaultTestLogger: LogDevice {
public:
    DefaultTestLogger();
    virtual ~DefaultTestLogger();

    static DefaultTestLogger& RunTimeLog();

    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level);

     template<class T>
     DefaultTestLogger& operator<<(const T& a) { 
         stringstream s;
         s << a;
         PRINT ( s.str() )
         return *this;
     }

     template<class T>
     inline DefaultTestLogger& operator<<(const char* s) { 
         PRINT ( s )
         return *this;
     }
     inline string str() const { 
         return testoutput.str(); 
     }

     // Handle endl
     static DefaultTestLogger& endl(DefaultTestLogger& stream)
     {
         return stream;
     }

    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

    // this is the function signature of std::endl
    typedef CoutType& (*StandardManip)(CoutType&);

    // define an operator<< to take in std::endl
    DefaultTestLogger& operator<<(StandardManip manip)
    {
        testoutput << manip;
        overview_log << manip;
        full_log << manip;
        PRINT ("\n")
        return *this;
    }

    void WriteLog(const string& fname, const Time& time);

    void ReportStringDiff(const string& expected, const string actual);

    /**
     * Log each queued message, and then clear the queue
     */
    void FlushQueue();
private:
    struct Message {
    	const Time time;
		const string message;
		const string context;
		const LOG_LEVEL level;
    };

    /**
     * Push the message out to all our consumers
     */
    void LogMessage(const Message& message);

    static DefaultTestLogger GLOBAL_LOG;
    stringstream testoutput;
    stringstream overview_log;
    stringstream full_log;
    ofstream     ffull_log;

    std::queue<Message>   messageQueue;

    bool        use_full_log;
    bool        use_capture_from;
};

#ifdef __PROFILE__TESTS
    typedef dummyLog testLogger;
#else
    typedef DefaultTestLogger testLogger;
#endif

enum TEST_TEMPLATE { BASIC, LOGGED };
typedef std::function<int(testLogger&)> loggedTest ;

class Test {
public:
    Test (string description, std::function<int(testLogger&)> test);
    enum FAILURE_ACTION {
        PRINT_LOG_AND_EXIT,
        PRINT_LOG_AND_THROW
    };

    struct TestFailureException {
        std::string description;
    };

    void RunTest(FAILURE_ACTION action = PRINT_LOG_AND_EXIT);
    double RunTime() { return runTime; }

private:
    std::function<int(testLogger& )> testLogged;
    string description;
	TEST_TEMPLATE testType;
    Time startTime;
    double runTime;
};


template<class T>
void FillContainer(T& container, int size, char filler) {
    for (int i = 0; i < size; i++) {
        container[i]  = filler;
    }
}

#endif
