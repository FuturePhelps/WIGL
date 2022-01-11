// Includes
#include "window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LPSTR ClassName = "Application";
HINSTANCE hAppInstance;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int CmdShow)
{
    MSG msg;
    HWND hWnd;

    hAppInstance = hInstance;

    if(!CreateApplicationWindow(WndProc, hAppInstance, ClassName, CmdShow, hWnd))
    {
        return 1;
    }

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_DESTROY:
            HGLRC currentContext = wglGetCurrentContext();  // Get the current OpenGL context
            wglMakeCurrent(GetDC(hWnd), NULL);              // Unset the OpenGL context
            wglDeleteContext(currentContext);               // Deleting the current OpenGL context
            PostQuitMessage(0);                             // Make sure the thread terminates
            return 0;                                       // Return with no errors
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}