#ifndef WINDOWH
#define WINDOWH

#include <windows.h>

/* 
Creates a WindowClass, registers it with the OS, creates a window and activates it.
Returns TRUE if the registration was successful, and returns FALSE if it was not.
Stores the resulting window handle into hWnd.

ARGUMENTS

WndProc: pointer to a window procedure function
hAppInstance: instance handle to the application provided by Windows through the WinMain function
ClassName: character string under which the window class is registered with the OS
CmdShow: int code provided by WinMain
hWnd: HWND variable where the newly created window handle is stored

RETURNS

TRUE if the window class registration was successful
FALSE if the window class registration was unsuccessful
*/
int CreateApplicationWindow(WNDPROC WndProc, HINSTANCE hAppInstance, LPSTR ClassName, int CmdShow, HWND& hWnd);

#endif