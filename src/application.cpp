// Includes
#include "window.h"
#include <gl/GL.h>

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

    bool running = true;

    while(running)
    {

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = false;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else
        {
            // TODO: Add some code here for rendering
        }
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        // On window deletion, delete OpenGL context and exit application
        case WM_DESTROY:
        {
            wglDeleteContext(wglGetCurrentContext());       // Deleting the current OpenGL context
            PostQuitMessage(0);                             // Make sure the thread terminates
            return 0;                                       // Return with no errors
        }

        // On window creation, initialize OpenGL context
        case WM_CREATE:
        {
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
            break;
        }

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}