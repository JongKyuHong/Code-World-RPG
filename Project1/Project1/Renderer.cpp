#include "Renderer.h"
#include <Windows.h>
#include <algorithm>
#include <string>
#include "TextLoader.h"

namespace
{
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
    : width(w), height(h), buffer(w* h, (WCHAR)ClearChar) // ClearChar(char) -> WCHAR로 변환
{
    textLoader = new TextLoader();
    HWND consoleWindow = GetConsoleWindow();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hOut == INVALID_HANDLE_VALUE)
        return;

    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    SetWindowLong(consoleWindow, GWL_STYLE, style & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);

    CONSOLE_CURSOR_INFO ci{};
    ci.dwSize = 1;
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ci);

    // 창 크기 바꿀 때 흔한 순서: window를 tiny로 줄이고 -> 버퍼 설정 -> window 다시 설정
    SMALL_RECT tiny{};
    tiny.Left = 0; tiny.Top = 0; tiny.Right = 0; tiny.Bottom = 0;
    SetConsoleWindowInfo(hOut, TRUE, &tiny);

    COORD bufferSize{ (SHORT)w, (SHORT)h };
    SetConsoleScreenBufferSize(hOut, bufferSize);

    SMALL_RECT targetRect = MakeClampedRect(hOut, (SHORT)w, (SHORT)h);
    SetConsoleWindowInfo(hOut, TRUE, &targetRect);
}

void Renderer::Present()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // 커서를 (0,0)으로 보내고 버퍼 전체를 한 번에 출력 (깜빡임 최소화)
    SetConsoleCursorPosition(hOut, { 0, 0 });

    std::wstring ws;
    ws.reserve((width + 1) * height);

    for (int y = 0; y < height; ++y)
    {
        ws.append(&buffer[y * width], width);
        ws.push_back(L'\n');
    }

    DWORD written = 0;
    WriteConsoleW(hOut, ws.c_str(), (DWORD)ws.size(), &written, nullptr);
}


void Renderer::Put(int x, int y, WCHAR c)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    // ASCII 기준 char -> WCHAR
    buffer[y * width + x] = c;
}

static std::wstring ToWideFromUtf8OrAcp(const std::string& s)
{
    if (s.empty()) return {};

    auto convert = [&](UINT cp, DWORD flags) -> std::wstring {
        int len = MultiByteToWideChar(cp, flags, s.c_str(), (int)s.size(), nullptr, 0);
        if (len <= 0) return {};
        std::wstring ws(len, L'\0');
        MultiByteToWideChar(cp, flags, s.c_str(), (int)s.size(), &ws[0], len);
        return ws;
        };

    // 1) UTF-8로 엄격하게 시도 (잘못된 UTF-8이면 무조건 실패)
    std::wstring ws = convert(CP_UTF8, MB_ERR_INVALID_CHARS);
    if (!ws.empty()) return ws;

    // 2) 실패하면 로컬 코드페이지(CP949 등)로 변환
    ws = convert(CP_ACP, 0);
    return ws;
}


void Renderer::PutString(int x, int y, const std::string& str) {
    if (y < 0 || y >= height) return;

    std::wstring ws = ToWideFromUtf8OrAcp(str);

    int start = (x < 0) ? 0 : x;
    int end = x + (int)ws.size();
    if (end > width) end = width;

    for (int i = start; i < end && i < width; ++i) {
        buffer[y * width + i] = ws[i - x];
    }
}


void Renderer::PutBox(int x, int y, int w, int h)
{
    if (w <= 0 || h <= 0) return;
    // 가로선
    for (int i = 0; i < w; ++i)
    {
        Put(x + i, y, L'─');
        Put(x + i, y + h - 1, L'─');
    }
    // 세로선
    for (int j = 0; j < h; ++j)
    {
        Put(x, y + j, L'│');
        Put(x + w - 1, y + j, L'│');
    }
    // 모서리
    Put(x, y, L'┌');
    Put(x + w - 1, y, L'┐');
    Put(x, y + h - 1, L'└');
    Put(x + w - 1, y + h - 1, L'┘');
}

void Renderer::PutTextFile(int x, int y, const std::string& filename)
{
    TextFile titleFile = textLoader->GetTexts(filename);
    for (int i = 0; i < titleFile.textLines.size(); i++) {
        PutString(x, y + i, std::string(titleFile.textLines[i].begin(), titleFile.textLines[i].end()));
    }
}

void Renderer::Clear()
{
    std::fill(buffer.begin(), buffer.end(), (WCHAR)ClearChar);
}

std::string Renderer::ToString() const
{
    // 이름 유지용: WCHAR -> char로 단순 변환(ASCII/1바이트 범위만 보장)
    std::string out;
    out.reserve((width + 1) * height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            WCHAR wc = buffer[y * width + x];
            char c = (wc <= 0xFF) ? (char)wc : '?';
            out.push_back(c);
        }
        out.push_back('\n');
    }
    return out;
}


TextFile Renderer::GetTextFile(const std::string& filename) {
    return textLoader->GetTexts(filename);
}

void Renderer::PutStringClipped(int x, int y, const std::string& str, int maxLen) {
    if (y < 0 || y >= height || maxLen <= 0) return;

    std::wstring ws = ToWideFromUtf8OrAcp(str);

    int start = (x < 0) ? 0 : x;
    int strEnd = x + (int)ws.size();
    int boxEnd = x + maxLen;  // 박스에서 허용하는 끝

    int end = strEnd;
    if (end > boxEnd) end = boxEnd;  // ✅ 박스 경계 강제!
    if (end > width) end = width;

    for (int i = start; i < end && i < width; ++i) {
        int srcIdx = i - x;
        if (srcIdx >= 0 && srcIdx < (int)ws.size()) {
            buffer[y * width + i] = ws[srcIdx];
        }
    }
}
