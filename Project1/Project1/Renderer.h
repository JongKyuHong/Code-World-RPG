#pragma once

#pragma once
#include<vector>
#include<string>

class Renderer {
public:
    Renderer(int w, int h);

    void Present();

    void Put(int x, int y, char c); // 특정 위치에 문자 그리기
    void Clear();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; };

    std::string ToString() const;

private:
    int width = 0;
    int height = 0;
    std::vector<char> buffer;

    const char ClearChar = ' ';
};