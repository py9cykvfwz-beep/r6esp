#include <windows.h>
#include <shellapi.h>
#include <tchar.h>
#include "shared.h"

// Function prototypes
void createMenu();
void injectCPA();
void setOverlayRect(int width, int height);

// Global variables
HINSTANCE hInstance;
HWND hwndTray;
NOTIFYICONDATA nid;
HANDLE hThread;
bool bRun = true;

// Entry point
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    // Initialize global variables
    this->hInstance = hInstance;

    // Create the menu and tray icon
    createMenu();

    // Start the overlay process
    injectCPA();

    // Main loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up
    Shell_NotifyIcon(NIM_DELETE, &nid);
    return 0;
}

// Function to create the menu and tray icon
void createMenu()
{
    // Initialize NOTIFYICONDATA
    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = CreateWindowEx(0, _T("STATIC"), _T(""), WS_POPUP, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_USER;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpy(nid.szTip, _T("R6 ESP"));
    Shell_NotifyIcon(NIM_ADD, &nid);

    // Create a context menu
    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, IDM_EXIT, _T("Exit"));
    SetMenuDefaultItem(hMenu, IDM_EXIT, FALSE);

    // Add a right-click context menu to the tray icon
    nid.uFlags |= NIF_CONTEXT;
    nid.hContextMenu = hMenu;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

// Function to inject the overlay process
void injectCPA()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the overlay process
    CreateProcess(NULL, _T("overlay.exe"), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    // Wait for the process to start
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

// Function to set the overlay rectangle
void setOverlayRect(int width, int height)
{
    // Implement the logic to send the width and height to the overlay process
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_USER:
        if (lParam == WM_RBUTTONDOWN)
        {
            // Show the context menu
            POINT pt;
            GetCursorPos(&pt);
            HMENU hMenu = (HMENU)wParam;
            SetForegroundWindow(hWndTray);
            TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWndTray, NULL);
            PostMessage(hWndTray, WM_NULL, 0, 0);
        }
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDM_EXIT)
        {
            // Exit the application
            bRun = false;
            PostQuitMessage(0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
