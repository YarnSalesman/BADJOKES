
/////////////////////////////////////   BLASTBTN   //////////////////////////////////////
#include <windows.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ucrt.lib")     // or msvcrt.lib

typedef struct _BLASTCANVAS {
    HDC     hdc;
    HBITMAP hbit;
    HBITMAP ohbit;
} BLASTCANVAS;

typedef struct _BLAST {
    BLASTCANVAS b1;
    BLASTCANVAS b2;
    BLASTCANVAS b3;
} BLAST;

#define ARRSIZE 0x324

typedef struct _BLASTINFO {
    RECT    rWin;
    HWND    hWinTarget;
    INT     WDIV20;
    INT     HDIV4;
    INT     x;
    INT     y;
    INT     width;
    INT     height;
    INT     DX [ARRSIZE];
    INT     DY [ARRSIZE];
    INT     SX [ARRSIZE];
    INT     SY [ARRSIZE];
    INT     RX [ARRSIZE];
    INT     RY [ARRSIZE];
} BLASTINFO;

static DWORD SEED;
static BOOL  RUNTHREAD;

DWORD GetRandom(DWORD range)
{
    SEED = SEED * 0x8088405 + 1;
    return (SEED * (DWORD64)range) >> 32;
}

VOID MakeBlast(BLASTINFO *bi)
{   
    bi->WDIV20  = bi->width/20;
    bi->HDIV4   = bi->height/4;

    INT i, j, b;
    for (j=0,b=0; j<4; j++)
    {
        for (i=0; i<20; i++,b++)
        {
            INT x = i*bi->WDIV20;
            INT y = j*bi->HDIV4;
        
            bi->DX[b] = x;
            bi->DY[b] = y;
            bi->SX[b] = x;
            bi->SY[b] = y;
            bi->RX[b] = (INT)GetRandom(10)-5;
            bi->RY[b] = (INT)GetRandom(10)-5;
        }
    }
}

VOID DrawBlast(BLASTINFO *bi, BLAST *b)
{
    HDC hdcScreen = GetDC(NULL);

    INT j;
    for (j=0; j<30; j++)
    {
        BitBlt(b->b3.hdc, 0, 0, bi->width+100, bi->height+100, b->b2.hdc, 0, 0, SRCCOPY);

        INT i;
        for (i=0; i<80; i++)
        {
            BitBlt(b->b3.hdc, bi->DX[i]+50, bi->DY[i]+50, bi->WDIV20*2, bi->HDIV4, b->b1.hdc, bi->SX[i], bi->SY[i], SRCCOPY);

            bi->DX[i] += bi->RX[i];
            bi->DY[i] += bi->RY[i];

            if (i==15 || i==20)
                bi->RY[i]++;

            if (i==22)
                bi->RX[i] = bi->RX[i]/2;
        }

        BitBlt(hdcScreen, bi->x-50, bi->y-50, bi->width+100, bi->height+100, b->b3.hdc, 0, 0, SRCCOPY);
        Sleep(10);
    }
    
    BitBlt(hdcScreen, bi->x-50, bi->y-50, bi->width+100, bi->height+100, b->b2.hdc, 0, 0, SRCCOPY);
    ReleaseDC(NULL, hdcScreen);

    SetWindowPos(GetParent(bi->hWinTarget),
                 NULL,
                 0, 0, 0, 0,
                 SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
}

DWORD WINAPI BlasterProc(LPVOID lParam)
{
    BLASTINFO   *bi;
    BLAST        b;
    HWND         hwnd;
    HDC          hdc;
    POINT        pos;

    bi = (BLASTINFO *)lParam;

    while (RUNTHREAD)
    {
        Sleep(50);
        GetCursorPos(&pos);

        hwnd = WindowFromPoint(pos);

        if (hwnd == bi->hWinTarget)
            continue;
        else
            bi->hWinTarget = hwnd;

        GetWindowRect(bi->hWinTarget, &bi->rWin);

        bi->x      = bi->rWin.left;
        bi->y      = bi->rWin.top;
        bi->width  = bi->rWin.right - bi->rWin.left;
        bi->height = bi->rWin.bottom - bi->rWin.top;

        // Skip anything larger than these dimensions
        if (bi->width  > 450) continue;
        if (bi->height > 100) continue;

        PlaySoundA("#100", NULL, SND_RESOURCE|SND_ASYNC);

        hdc        = GetDC(bi->hWinTarget);

        b.b1.hdc   = CreateCompatibleDC(hdc);
        b.b1.hbit  = CreateCompatibleBitmap(hdc, bi->width, bi->height);
        b.b1.ohbit = SelectObject(b.b1.hdc, b.b1.hbit);
        
        b.b2.hdc   = CreateCompatibleDC(hdc);
        b.b2.hbit  = CreateCompatibleBitmap(hdc, bi->width+100, bi->height+100);
        b.b2.ohbit = SelectObject(b.b2.hdc, b.b2.hbit);

        b.b3.hdc   = CreateCompatibleDC(hdc);
        b.b3.hbit  = CreateCompatibleBitmap(hdc, bi->width+100, bi->height+100);
        b.b3.ohbit = SelectObject(b.b3.hdc, b.b3.hbit);

        BitBlt(b.b1.hdc, 0, 0, bi->width, bi->height, hdc, 0, 0, SRCCOPY);

        ReleaseDC(bi->hWinTarget, hdc);

        ShowWindow(bi->hWinTarget, SW_HIDE);

        Sleep(50);

        hdc = GetDC(NULL);

        BitBlt(b.b2.hdc, 0, 0, bi->width+100, bi->height+100, hdc, bi->x-50, bi->y-50, SRCCOPY);

        ReleaseDC(NULL, hdc);

        MakeBlast(bi);

        DrawBlast(bi, &b);

        SelectObject(b.b1.hdc, b.b1.ohbit);
        SelectObject(b.b2.hdc, b.b2.ohbit);
        SelectObject(b.b3.hdc, b.b3.ohbit);
        DeleteObject(b.b1.hbit);
        DeleteObject(b.b2.hbit);
        DeleteObject(b.b3.hbit);
        DeleteDC(b.b1.hdc);
        DeleteDC(b.b2.hdc);
        DeleteDC(b.b3.hdc);
    }
    return 0;
}

VOID wmain()
{
    // GCC might not recognize MOD_NOREPEAT(0x00004000)
    if (!RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_ESCAPE))
        ExitProcess(0);

    BLASTINFO bi = { 0 }; //GCC doesn't like the 0. Use { }.

    SEED         = GetTickCount();
    RUNTHREAD    = TRUE;

    HANDLE hThread = CreateThread(NULL, 0, BlasterProc, &bi, 0, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            RUNTHREAD = FALSE;
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
            UnregisterHotKey(NULL, 1);
            break;
        }
    }

    ExitProcess(0);
}
