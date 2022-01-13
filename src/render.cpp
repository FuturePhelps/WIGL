#include "render.h"
#include <gl/GL.h>
#include <windows.h>

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