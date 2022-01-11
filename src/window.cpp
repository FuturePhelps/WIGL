#include "window.h"

int CreateApplicationWindow(WNDPROC WndProc, HINSTANCE hAppInstance, LPSTR ClassName, int CmdShow, HWND& hWnd)
{
    // Create the window class struct
    WNDCLASS wc;

    // Fill in the window class fields
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // Window class style (bit flags)
    wc.lpfnWndProc = WndProc;                           // Pointer to window procedure function
    wc.cbClsExtra = 0;                                  // Number of extra bytes to allocate for this window class
    wc.cbWndExtra = 0;                                  // Number of extra bytes to allocate for each instance of this window class
    wc.hInstance = hAppInstance;                        // Handle to the application instance
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);         // Executable icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);           // Cursor while inside an instance of this window class
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);  // Background color for the window if no drawing is done
    wc.lpszMenuName = NULL;                             // Menu window name
    wc.lpszClassName = ClassName;                       // The class name under which this window class is registered

    // Register the window class to the OS
    if(!RegisterClass(&wc))
    {
        // If it fails, display a message box and return FALSE
        MessageBox(NULL, "Failed to register window class", "Error", MB_OK);
        return FALSE;
    }
    
    // Create the window and store the handle into hWnd
    hWnd = CreateWindow(ClassName,                                                  // Window class for this instance
                        "Application",                                              // Text that displays at the top of the window
                        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,    // Window style (bit flags)
                        CW_USEDEFAULT,                                              // x position
                        CW_USEDEFAULT,                                              // y position
                        CW_USEDEFAULT,                                              // Width in pixels
                        CW_USEDEFAULT,                                              // Height in pixels
                        NULL,                                                       // Window handle for the parent window
                        NULL,                                                       // Handle to the menu window
                        hAppInstance,                                               // Handle to the application instance
                        NULL                                                        // Pointer to a value to be passed down to the window
                        );
    
    // Show and update the window before returning
    ShowWindow(hWnd, CmdShow);
    UpdateWindow(hWnd);

    // Return TRUE if this function ran successfully
    return TRUE;
}