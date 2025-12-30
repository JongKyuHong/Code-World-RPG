#include "Renderer.h"
#include <Windows.h>
#include <iostream>

Renderer::Renderer(int w, int h) :width(w), height(h), buffer(w* h, ClearChar)
{
}

void Renderer::Present()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    WriteConsoleA(hOut, buffer.data(), buffer.size(), nullptr, nullptr);
}

void Renderer::Put(int x, int y, char c)
{
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }

    buffer[y * width + x] = c;
}

void Renderer::Clear()
{
    std::fill(buffer.begin(), buffer.end(), ClearChar);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD home = { 0, 0 };
    SetConsoleCursorPosition(hOut, home);
}

std::string Renderer::ToString() const
{
    std::string out;
    out.reserve((width + 1) * height);

    for (int y = 0; y < height; ++y) {
        out.append(buffer.data() + y * width, width);
        out.push_back('\n');
    }
    return out;
}
