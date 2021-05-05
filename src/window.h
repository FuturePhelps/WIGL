#ifndef WINDOWH
#define WINDOWH

#include <windows.h>

int CreateApplicationWindow(WNDPROC WndProc, HINSTANCE hAppInstance, LPSTR ClassName, int CmdShow, HWND& hWnd);

#endif