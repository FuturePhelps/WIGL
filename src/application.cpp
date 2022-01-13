// Includes
#include "wndmessages.h"
#include "params.h"
#include "window.h"
#include <gl/GL.h>
#include "render.h"

LPSTR ClassName = "Application";
HINSTANCE hAppInstance;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int CmdShow)
{
    // Set up the windowing variables
    MSG msg;
    HWND hWnd;

    // Set our app instance to the instance given by Windows
    hAppInstance = hInstance;

    // Create our application window
    if(!CreateApplicationWindow(WndProc, hAppInstance, ClassName, CmdShow, hWnd))
    {
        return 1;
    }

    // Keep track of when the program is running for the loop
    bool running = true;

    // For the timer
    const UINT msPerFrame = (UINT)(1.0e3 / maxFPS);

    // Set a timer for the window at the specified framerate
    UINT_PTR timer = SetTimer(hWnd, NULL, msPerFrame, NULL);

    // Message and rendering loop
    while(running)
    {
        // Check if there are messages to process and send them to WndProc if so
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Stop the loop on WM_QUIT message
            if (msg.message == WM_QUIT)
            {
                KillTimer(hWnd, timer);
                running = false;
                break;
            }

            // Send messages to WndProc
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Return the wParam of the last message (Should be 0 if everything went well)
    return msg.wParam;
}