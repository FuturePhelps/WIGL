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


    // Create a Pixel Format Descriptor for the device context
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),                                  // Size (bytes)
        1,                                                              // Version
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,     // Flags
        PFD_TYPE_RGBA,                                                  // Framebuffer type, RGBA or palette.
        32,                                                             // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,                                               // RedBits, RedShift, GreenBits, GreenShift, BlueBits, BlueShift
        0,                                                              // AlphaBits
        0,                                                              // AlphaShift
        0,                                                              // Number of bitplanes in the accumulation buffer
        0, 0, 0, 0,                                                     // Number of bitplanes in each RGBA accumulation buffer
        24,                                                             // Number of bits for the depthbuffer
        8,                                                              // Number of bits for the stencilbuffer
        0,                                                              // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,                                                 // LayerType (deprecated)
        0,                                                              // Number of overlay and underlay planes
        0, 0, 0                                                         // LayerMask (deprecated), VisibleMask (transparent RGB value), DamageMask (deprecated)
    };

    // Register the window class to the OS
    if(!RegisterClass(&wc))
    {
        // If it fails, display a message box and return FALSE
        MessageBox(NULL, "Failed to register window class", "Error", MB_OK);
        return FALSE;
    }
    
    // Create the window and store the handle into hWnd
    hWnd = CreateWindow(ClassName,                      // Window class for this instance
                        "Application",                  // Text that displays at the top of the window
                        WS_OVERLAPPEDWINDOW,            // Window style (bit flags)
                        CW_USEDEFAULT,                  // x position
                        CW_USEDEFAULT,                  // y position
                        CW_USEDEFAULT,                  // Width in pixels
                        CW_USEDEFAULT,                  // Height in pixels
                        NULL,                           // Window handle for the parent window
                        NULL,                           // Handle to the menu window
                        hAppInstance,                   // Handle to the application instance
                        NULL                            // Pointer to a value to be passed down to the window
                        );


    // Get the device context from the window handle
    HDC hdc = GetDC(hWnd);
    
    // Find the best pixel format given the parameters of pfd
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);

    // Set the pixel format to the window's device context
    if(!SetPixelFormat(hdc, pixelFormat, &pfd))
    {
        // If it fails, display a message box and return FALSE
        MessageBox(NULL, "Failed to set pixel format", "Error", MB_OK);
        return FALSE;
    }

    // Get the OpenGL context
    HGLRC context = wglCreateContext(hdc);

    // Make the context current
    wglMakeCurrent(hdc, context);
    
    // Show and update the window before returning
    ShowWindow(hWnd, CmdShow);
    UpdateWindow(hWnd);

    // Return TRUE if this function ran successfully
    return TRUE;
}