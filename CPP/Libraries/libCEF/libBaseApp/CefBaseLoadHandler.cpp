/*
 * CefBaseLoadHandler.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseLoadHandler.h"

CefBaseLoadHandler::CefBaseLoadHandler() {

}

CefBaseLoadHandler::~CefBaseLoadHandler() {
}

void CefBaseLoadHandler::OnLoadError(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    ErrorCode errorCode,
    const CefString& errorText,
    const CefString& failedUrl)
{
    for (PointerType& handler: handlers) {
        handler->OnLoadError(browser,frame,errorCode,errorText,failedUrl);
    }
}
