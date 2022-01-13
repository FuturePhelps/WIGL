// Includes
#include "window.h"
#include <gl/GL.h>
#include "glcorearb.h"
#include "glext.h"
#include "wglext.h"

// Parameters for the app
static const double maxFPS = 60.0;
static const double aspectRatio = 16.0/9.0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LPSTR ClassName = "Application";
HINSTANCE hAppInstance;

// Shows the render area in white
void render(HWND hWnd)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 0);
    glVertex3f(1.0, 1.0, 0);
    glVertex3f(1.0, -1.0, 0);
    glVertex3f(-1.0, -1.0, 0);
    glEnd();
    SwapBuffers(GetDC(hWnd));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int CmdShow)
{
    // Set up the windowing variables
    MSG msg;
    HWND hWnd;

    hAppInstance = hInstance;

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

            // Rendering happens on the WM_TIMER message
            // We also render on WM_PAINT to stop flickering when resizing

            // Send messages to WndProc
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Return the wParam of the last message (Should be 0 if everything went well)
    return msg.wParam;
}

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
            ReleaseDC(hWnd, GetDC(hWnd));           // Release the device context
            wglDeleteContext(wglGetCurrentContext());      // Deleting the current OpenGL context
            PostQuitMessage(0);             // Make sure the thread terminates
            return 0;                       // Return with no errors
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