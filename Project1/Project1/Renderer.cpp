#include "Renderer.h"
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
    : width(w), height(h), buffer(w* h, (WCHAR)ClearChar), colorBuffer(w* h, LIGHT_GRAY)
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

    SMALL_RECT tiny{};
    tiny.Left = 0; tiny.Top = 0; tiny.Right = 0; tiny.Bottom = 0;
    SetConsoleWindowInfo(hOut, TRUE, &tiny);

    COORD bufferSize{ (SHORT)w, (SHORT)h };
    SetConsoleScreenBufferSize(hOut, bufferSize);

    SMALL_RECT targetRect = MakeClampedRect(hOut, (SHORT)w, (SHORT)h);
    SetConsoleWindowInfo(hOut, TRUE, &targetRect);

    SetConsoleTextAttribute(hOut, (BLACK << 4) | WHITE);
}



// 색상 정보와 함께 한 번에 출력 (깜빡임 최소화)
void Renderer::Present()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, { 0, 0 });

    // [핵심] 콘솔 전체 크기에 맞춰 screenBuffer를 동적으로 생성
    int consoleWidth = csbi.dwSize.X;
    int consoleHeight = csbi.dwSize.Y;
    std::vector<CHAR_INFO> screenBuffer(consoleWidth * consoleHeight);

    // 1. 먼저 콘솔 전체를 buffer 내용으로 채움 (공백으로)
    for (int y = 0; y < consoleHeight; ++y) {
        for (int x = 0; x < consoleWidth; ++x) {
            int idx = y * consoleWidth + x;

            // 게임 영역(width x height) 안이면 실제 buffer 내용, 
            // 바깥 영역이면 공백(LIGHT_GRAY)으로 채움
            if (y < height && x < width) {
                int gameIdx = y * width + x;
                screenBuffer[idx].Char.UnicodeChar = buffer[gameIdx];
                screenBuffer[idx].Attributes = (BLACK << 4) | colorBuffer[gameIdx];
            } else {
                // 게임 영역 밖은 공백으로 채움 (똥값 방지)
                screenBuffer[idx].Char.UnicodeChar = ' ';
                screenBuffer[idx].Attributes = LIGHT_GRAY;
            }
        }
    }

    // 2. 콘솔 전체 영역에 WriteConsoleOutput
    COORD bufferSize = { (SHORT)consoleWidth, (SHORT)consoleHeight };
    COORD origin = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, (SHORT)(consoleWidth - 1), (SHORT)(consoleHeight - 1) };

    WriteConsoleOutput(hOut, screenBuffer.data(), bufferSize, origin, &writeRegion);
}


void Renderer::Put(int x, int y, WCHAR c)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    buffer[y * width + x] = c;
    colorBuffer[y * width + x] = currentColor;
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

    std::wstring ws = convert(CP_UTF8, MB_ERR_INVALID_CHARS);
    if (!ws.empty()) return ws;

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
        int idx = y * width + i;
        buffer[idx] = ws[i - x];
        colorBuffer[idx] = currentColor;
    }
}

void Renderer::SetColor(Color foreground, Color background) {
    currentColor = foreground;
}

void Renderer::ResetColor() {
    currentColor = BLACK;
}

void Renderer::PutString(int x, int y, const std::string& str, Color color) {
    if (y < 0 || y >= height) return;

    Color oldColor = currentColor;
    currentColor = color;

    std::wstring ws = ToWideFromUtf8OrAcp(str);

    int start = (x < 0) ? 0 : x;
    int end = x + (int)ws.size();
    if (end > width) end = width;

    for (int i = start; i < end && i < width; ++i) {
        int idx = y * width + i;
        buffer[idx] = ws[i - x];
        colorBuffer[idx] = color;
    }

    currentColor = oldColor;
}

// ════════════════════════════════════════════════════════════

void Renderer::PutBox(int x, int y, int w, int h)
{
    if (w <= 0 || h <= 0) return;
    for (int i = 0; i < w; ++i)
    {
        Put(x + i, y, L'─');
        Put(x + i, y + h - 1, L'─');
    }
    for (int j = 0; j < h; ++j)
    {
        Put(x, y + j, L'│');
        Put(x + w - 1, y + j, L'│');
    }
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
    std::fill(colorBuffer.begin(), colorBuffer.end(), LIGHT_GRAY);
}



std::string Renderer::ToString() const
{
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
    int boxEnd = x + maxLen;

    int end = strEnd;
    if (end > boxEnd) end = boxEnd;
    if (end > width) end = width;

    for (int i = start; i < end && i < width; ++i) {
        int srcIdx = i - x;
        if (srcIdx >= 0 && srcIdx < (int)ws.size()) {
            buffer[y * width + i] = ws[srcIdx];
            colorBuffer[y * width + i] = currentColor;
        }
    }
}
