#include "Renderer.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

namespace
{
    void PrintWinApiError(const char* where)
    {
        DWORD err = GetLastError();
        if (err == 0) return;

        LPSTR msg = nullptr;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&msg, 0, nullptr);

        if (msg) LocalFree(msg);
    }

    bool TrySetConsoleFontSmall(HANDLE hOut, const wchar_t* faceName, SHORT fontY)
    {
        CONSOLE_FONT_INFOEX cfi{};
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;     // 0이면 시스템이 적절히
        cfi.dwFontSize.Y = fontY; // 중요: 높이를 줄일수록 더 큰 콘솔 격자 가능
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcsncpy_s(cfi.FaceName, faceName, _TRUNCATE);

        if (!SetCurrentConsoleFontEx(hOut, FALSE, &cfi))
        {
            PrintWinApiError("SetCurrentConsoleFontEx");
            return false;
        }
        return true;
    }

    SMALL_RECT MakeClampedRect(HANDLE hOut, SHORT w, SHORT h)
    {
        COORD largest = GetLargestConsoleWindowSize(hOut);
        if (largest.X <= 0) largest.X = w;
        if (largest.Y <= 0) largest.Y = h;

        SHORT cw = (w <= largest.X) ? w : largest.X;
        SHORT ch = (h <= largest.Y) ? h : largest.Y;

        SMALL_RECT r{};
        r.Left = 0;
        r.Top = 0;
        r.Right = static_cast<SHORT>(cw - 1);
        r.Bottom = static_cast<SHORT>(ch - 1);
        return r;
    }
}

Renderer::Renderer(int w, int h)
    : width(w), height(h), buffer(w* h, ClearChar)
{
    HWND consoleWindow = GetConsoleWindow();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hOut == INVALID_HANDLE_VALUE)
    {
        PrintWinApiError("GetStdHandle(STD_OUTPUT_HANDLE)");
        return;
    }

    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    SetWindowLong(consoleWindow, GWL_STYLE, style & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);

    CONSOLE_CURSOR_INFO ci{};
    ci.dwSize = 1;
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ci);

    SMALL_RECT tiny{};
    tiny.Left = 0; tiny.Top = 0; tiny.Right = 0; tiny.Bottom = 0;
    if (!SetConsoleWindowInfo(hOut, TRUE, &tiny))
        PrintWinApiError("SetConsoleWindowInfo(tiny)");

    COORD bufferSize{ static_cast<SHORT>(w), static_cast<SHORT>(h) };
    if (!SetConsoleScreenBufferSize(hOut, bufferSize))
        PrintWinApiError("SetConsoleScreenBufferSize(target)");

    SMALL_RECT targetRect = MakeClampedRect(hOut, static_cast<SHORT>(w), static_cast<SHORT>(h));
    if (!SetConsoleWindowInfo(hOut, TRUE, &targetRect))
        PrintWinApiError("SetConsoleWindowInfo(target)");
}

void Renderer::Present()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string s = ToString();
    WriteConsoleA(hOut, s.c_str(), static_cast<DWORD>(s.size()), nullptr, nullptr);
}

void Renderer::Put(int x, int y, char c)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    buffer[y * width + x] = c;
}

void Renderer::Clear()
{
    std::fill(buffer.begin(), buffer.end(), ClearChar);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, { 0, 0 });
}

std::string Renderer::ToString() const
{
    std::string out;
    out.reserve((width + 1) * height);

    for (int y = 0; y < height; ++y)
    {
        out.append(buffer.data() + y * width, width);
        out.push_back('\n');
    }
    return out;
}
