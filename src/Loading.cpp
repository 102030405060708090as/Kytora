#include <windows.h>
#include <gdiplus.h>
#include <thread>

using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")

int progress = 0;
bool loadingDone = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void LoadSimulation(HWND hwnd) {
    for (int i = 0; i <= 100; i++) {
        progress = i;
        InvalidateRect(hwnd, NULL, TRUE);
        Sleep(30);  // ÓÑÚÉ ÇáÊÍãíá
    }
    loadingDone = true;
    // åäÇ ÊÝÊÍ ÇáÕÝÍÉ ÇáÊÇáíÉ
    MessageBox(hwnd, L"ÊÍãíá ÇáäÙÇã ÇäÊåì!", L"Kytora", MB_OK);
    PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"KytoraLoading";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST,
        L"KytoraLoading", L"Kytora",
        WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    std::thread loader(LoadSimulation, hwnd);
    loader.detach();

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}

void DrawCenteredImage(HDC hdc, LPCWSTR imagePath, int screenWidth, int screenHeight) {
    Graphics graphics(hdc);
    Image image(imagePath);
    int imgWidth = image.GetWidth();
    int imgHeight = image.GetHeight();
    int x = (screenWidth - imgWidth) / 2;
    int y = (screenHeight - imgHeight) / 2 - 50;
    graphics.DrawImage(&image, x, y);
}

void DrawProgressBar(HDC hdc, int width, int height, int progress) {
    int barWidth = width / 2;
    int barHeight = 20;
    int x = (width - barWidth) / 2;
    int y = height / 2 + 100;

    HBRUSH bgBrush = CreateSolidBrush(RGB(50, 50, 50));
    HBRUSH fgBrush = CreateSolidBrush(RGB(0, 120, 215));

    RECT bg = { x, y, x + barWidth, y + barHeight };
    FillRect(hdc, &bg, bgBrush);

    RECT fg = { x, y, x + (barWidth * progress / 100), y + barHeight };
    FillRect(hdc, &fg, fgBrush);

    DeleteObject(bgBrush);
    DeleteObject(fgBrush);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0))));
        DrawCenteredImage(hdc, L"assets/logo.png", width, height);
        DrawProgressBar(hdc, width, height, progress);

        EndPaint(hwnd, &ps);
    }
                 return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
