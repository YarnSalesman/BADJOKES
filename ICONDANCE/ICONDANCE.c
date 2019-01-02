
//////////////////////////////////////  ICONDANCE  //////////////////////////////////////
#include <windows.h>
#include <commctrl.h>
#include <math.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "ucrt.lib")     // or msvcrt.lib


typedef struct _CPNODE {
    POINT   p;
    struct  _CPNODE *next;
    struct  _CPNODE *prev;
} CPNODE;

typedef struct _FOLLOWCURSOR {
    INT      MSGID;
    INT      dwIconCount;
    HWND     hDesktop;
    CPNODE  *CCoords;
} FOLLOWCURSOR;


BOOL CALLBACK FindSDV(HWND hWnd, LPARAM lParam)
{
    HWND hShellDefView = FindWindowExW(hWnd, NULL, L"SHELLDLL_DefView", NULL);
    if (hShellDefView)
    {
        *((HWND *)lParam) = hShellDefView;
        return FALSE;
    }
    return TRUE;
}

HWND GetDesktopListView()
{
    HWND hProgman      = FindWindowW(L"Progman", NULL);
    HWND hShellDefView = FindWindowExW(hProgman, NULL, L"SHELLDLL_DefView", NULL);

    if (!hShellDefView)
    {
        EnumWindows(FindSDV, (LPARAM)&hShellDefView);
        if (!hShellDefView)
            return NULL;
    }

    return FindWindowExW(hShellDefView, NULL, L"SysListView32", NULL);
}

DWORD WINAPI FollowProc(LPVOID lpParam)
{
    INT            icon;
    POINT          cpos;
    CPNODE         *curr, *pos;
    FOLLOWCURSOR   *fc;

    fc  = (FOLLOWCURSOR *)lpParam;
    pos = fc->CCoords;

    while(fc->MSGID == 0)
    {
        GetCursorPos(&cpos);
        if (cpos.x != pos->p.x && cpos.y != pos->p.y)
        {
            pos      = pos->next;
            pos->p.x = cpos.x;
            pos->p.y = cpos.y;
            curr     = pos;

            for (icon=0; icon<fc->dwIconCount; icon++)
            {
                ListView_SetItemPosition(fc->hDesktop, icon, pos->p.x, pos->p.y);
                pos = pos->prev;
                if (pos->p.x == 0 && pos->p.y == 0)
                {
                    pos = curr;
                    break;
                }
            }
        }
    }

    return 0;
}

VOID IconFollow(HWND hDesktop, INT icons)
{
    HANDLE          hThread;

    FOLLOWCURSOR    fc;
    fc.MSGID        = 0;
    fc.hDesktop     = hDesktop;
    fc.dwIconCount  = icons;
    fc.CCoords      = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(CPNODE) * icons);

    // Make a circular doubly-linked list to hold cursor positions.
    CPNODE          *pos;
    pos             = fc.CCoords;
    pos->prev       = pos + (icons - 1);
    pos->prev->next = pos;

    while (pos->next == NULL)
    {
        pos->next = pos + 1;
        pos = pos->next;
        pos->prev = pos - 1;
    }

    GetCursorPos(&fc.CCoords->p);

    hThread = CreateThread(NULL, 0, FollowProc, &fc, 0, NULL);
    if (NULL == hThread)
    {
        MessageBoxW(NULL, L"Thread could not be created!", L"Error", MB_ICONERROR | MB_SETFOREGROUND);
    }
    else
    {
        fc.MSGID = MessageBoxW(NULL, L"Now you can reorganize your icons!", L"Icondance", MB_OK);

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    HeapFree(GetProcessHeap(), 0, fc.CCoords);
}

VOID IconDance()
{
    HWND   hDesktop = GetDesktopListView();
    INT    h        = GetSystemMetrics(SM_CYSCREEN);
    INT    w        = GetSystemMetrics(SM_CXSCREEN);
    INT    icons    = ListView_GetItemCount(hDesktop);
    INT    duration = (w/3)+1;
    INT    m, n, a;
    double x, y;

    for (m=0; m<duration; m++)
    {
        for (n=0; n<icons; n++)
        {
            a = (m*3) - ((n+n)*5);
            x = ((sin((double)a/25.0)+2.0)*w)/4.0;
            x = ((sin((x/2.0)/100.0)+2.0)*h)/4.0;
            y = ((sin((double)a/100.0)+2.0)*h)/4.0;
            y = ((sin(y/100.0)+2.0)*h)/4.0;
            ListView_SetItemPosition(hDesktop, n, x, y);
        };
    }

    for (m=0; m<duration; m++)
    {
        for (n=0; n<icons; n++)
        {
            a = (m*3) - ((n+n)*5);
            x = ((sin((double)a/25.0)+2.0)*w)/4.0;
            y = ((sin((double)a/100.0)+2.0)*h)/4.0;
            ListView_SetItemPosition(hDesktop, n, x, y);
        }
    }

    for (m=0; m<duration; m++)
    {
        for (n=0; n<icons; n++)
        {
            a = (m*3) - (n*32);
            x = (((cos((double)a/100.0)*((double)n/20.0))+1.0)*w)/2.0;
            y = (((sin((double)a/100.0)*((double)n/20.0))+1.0)*h)/2.0;
            ListView_SetItemPosition(hDesktop, n, x, y);
        }
    }

    IconFollow(hDesktop, icons);
}

BOOL CALLBACK Minimize(HWND hwnd, LPARAM lParam)
{
    if (!IsIconic(hwnd) && IsWindowVisible(hwnd))
        CloseWindow(hwnd);
    return -1;
}

VOID wmain()
{
    EnumWindows(Minimize, 0);
    IconDance();
    ExitProcess(0);
}
