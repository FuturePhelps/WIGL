// Includes
#include "window.h"
#include <gl/GL.h>
#include "glcorearb.h"
#include "glext.h"
#include "wglext.h"
#include <chrono>

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

    HDC hdc = GetDC(hWnd);

    // Keep track of when the program is running for the loop
    bool running = true;

    // Some declarations for framerate limiter
    std::chrono::steady_clock::time_point lastRender = std::chrono::high_resolution_clock::now();
    std::chrono::steady_clock::time_point currentTime;
    long long dt;
    const double maxFPS = 60.0;
    const long long microsecondsPerFrame = (long long)(1.0e6 / maxFPS);
    

    // Message and rendering loop
    while(running)
    {
        // Check if there are messages to process and send them to WndProc if so
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Stop the loop on WM_QUIT message
            if (msg.message == WM_QUIT)
            {
                running = false;
                break;
            }

            // Send messages to WndProc
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Get the current time and calculate the time since the last render
        currentTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastRender).count();
        
        if (dt >= microsecondsPerFrame)
        {
            // Render here
            glClearColor(0.0, 0.0, 0.0, 0.0)
            glClear(GL_COLOR_BUFFER_BIT);
            glBegin(GL_TRIANGLES);
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(0, 0, 0);
            glVertex3f(1, 0, 0);
            glVertex3f(0, 1, 0);
            glEnd();
            SwapBuffers(hdc);

            // Update the time
            lastRender = currentTime;
        }
    }

    // Return the wParam of the last message (Should be 0 if everything went well)
    return msg.wParam;
}

// Window Procedure function to handle messages to the window
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Define some variables
    HDC hdc;
    int pixelFormat;
    HGLRC context;

    switch(msg)
    {
        // On window deletion, delete OpenGL context and exit application
        case WM_DESTROY:
        {
            ReleaseDC(hWnd, hdc);           // Release the device context
            wglDeleteContext(context);      // Deleting the current OpenGL context
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
            hdc = GetDC(hWnd);
            
            // Find the best pixel format given the parameters of pfd
            pixelFormat = ChoosePixelFormat(hdc, &pfd);

            // Set the pixel format to the window's device context
            if(!SetPixelFormat(hdc, pixelFormat, &pfd))
            {
                // If it fails, display a message box and return FALSE
                MessageBox(NULL, "Failed to set pixel format", "Error", MB_OK);
                return FALSE;
            }

            // Get the OpenGL context
            context = wglCreateContext(hdc);

            // Make the context current
            wglMakeCurrent(hdc, context);
            break;
        }

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}