// Includes
#include "wndmessages.h"
#include <windows.h>
#include "params.h"
#include <gl/GL.h>
#include "render.h"

// Window Procedure function to handle messages to the window
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        // On WM_TIMER, render
        case WM_TIMER:
        {
            // This is just a test render
            render(hWnd);
            break;
        }

        // Render on WM_PAINT
        case WM_PAINT:
        {
            render(hWnd);
            break;
        }

        // Handle resizing
        case WM_SIZE:
        {
            // Get the new width and height
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);

            // Set up some variables
            UINT temp;
            GLint x = 0;
            GLint y = 0;

            // Keep the aspect ratio constant
            if ((double)width / height >= aspectRatio)
            {
                temp = (UINT)((double)height * aspectRatio);
                x = (width - temp) / 2;
                width = temp;
            }
            else
            {
                temp = (UINT)((double)width / aspectRatio);
                y = (height - temp) / 2;
                height = temp;
            }

            // Change the viewport
            glViewport(x, y, width, height);
            break;
        }

        // On window deletion, delete OpenGL context and exit application
        case WM_DESTROY:
        {
            ReleaseDC(hWnd, GetDC(hWnd));               // Release the device context
            wglDeleteContext(wglGetCurrentContext());   // Deleting the current OpenGL context
            PostQuitMessage(0);                         // Make sure the thread terminates
            return 0;                                   // Return with no errors
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

            // Set the clear color to black
            glClearColor(0.0, 0.0, 0.0, 0.0);
            break;
        }

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}