#include "window.h"

int CreateApplicationWindow(WNDPROC WndProc, HINSTANCE hAppInstance, LPSTR ClassName, int CmdShow, HWND& hWnd)
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hAppInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = ClassName;

    if(!RegisterClass(&wc))
    {
        MessageBox(NULL, "This program required Windows NT", "Error", MB_OK);
        return FALSE;
    }

    hWnd = CreateWindow(ClassName,
                        "Application",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        NULL,
                        NULL,
                        hAppInstance,
                        NULL
                        );
    
    ShowWindow(hWnd, CmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}