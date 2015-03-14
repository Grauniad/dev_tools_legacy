/*
 * GCGVBrowser_Callbacks_Display.cpp
 *
 *  Created on: 8 Feb 2015
 *      Author: lhumphreys
 */

#include "CefBaseDisplayGTKDefaultHandler.h"

#include <string>
#include "GTKUtils.h"
#include "include/wrapper/cef_helpers.h"

CefBaseDisplayDefaultGTKHandler::CefBaseDisplayDefaultGTKHandler() {
}

CefBaseDisplayDefaultGTKHandler::~CefBaseDisplayDefaultGTKHandler() {
}

void CefBaseDisplayDefaultGTKHandler::OnTitleChange(
		CefRefPtr<CefBrowser> browser,
		const CefString& title)
{
	CEF_REQUIRE_UI_THREAD();
	std::string titleStr(title);

	// Retrieve the X11 display shared with Chromium.
	Display* display = cef_get_xdisplay();
	DCHECK(display);

	// Retrieve the X11 window handle for the browser.
	Window window = browser->GetHost()->GetWindowHandle();
	DCHECK(window != kNullWindowHandle);

	// Retrieve the atoms required by the below XChangeProperty call.
    GTKUtils::SetWindowTitle(window, titleStr, display);
}
