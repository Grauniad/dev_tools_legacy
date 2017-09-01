#include "CefBaseMainLoop.h"

#include <X11/Xlib.h>

#include "include/cef_app.h"
#include "include/cef_client.h"

namespace {
    int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
      LOG(WARNING)
            << "X error received: "
            << "type " << event->type << ", "
            << "serial " << event->serial << ", "
            << "error_code " << static_cast<int>(event->error_code) << ", "
            << "request_code " << static_cast<int>(event->request_code) << ", "
            << "minor_code " << static_cast<int>(event->minor_code);
      return 0;
    }

    int XIOErrorHandlerImpl(Display *display) {
      return 0;
    }
}

int CefBaseAppUtils::Main(int argc, char* argv[], CefRefPtr<CefBaseApp> app) {
    // Provide CEF with command-line arguments.  
    CefMainArgs main_args(argc, argv);

    /************************************************************
     *                  Sub-process Handling
     ************************************************************/

    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(main_args, app, NULL);

    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        return exit_code;
    }

    /************************************************************
     *                  Main (Browser) Handling
     ************************************************************/

    CefSettings settings;

    if ( !settings.remote_debugging_port)
    {
        settings.remote_debugging_port = 9222;
    }

    // Install xlib error handlers so that the application won't be terminated
    // on non-fatal errors.
    XSetErrorHandler(XErrorHandlerImpl);
    XSetIOErrorHandler(XIOErrorHandlerImpl);

    // Initialize CEF for the browser process.
    CefInitialize(main_args, settings, app, NULL);

    // Run the CEF message loop. This will block until CefQuitMessageLoop() is
    // called.
    CefRunMessageLoop();

    CefShutdown();

    return 0;
}


