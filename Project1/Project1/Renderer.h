#pragma once

#include <Windows.h>   // WCHAR 정의
#include <vector>
#include <string>
struct TextFile;
class TextLoader;
class Renderer {
public:
    Renderer(int w, int h);

    void Present(); // Buffer에 있는 것을 화면에 출력

    void Put(int x, int y, WCHAR c);                    // 특정 위치에 문자 그리기
    void PutString(int x, int y, const std::string&);  // 특정 위치에 문자열 그리기
    void PutBox(int x, int y, int w, int h);               // 특정 위치에 박스 그리기
    void PutTextFile(int x, int y, const std::string& filename); // 특정 위치에 텍스트 파일 그리기

    void Clear();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    std::string ToString() const;

    TextLoader* GetTextLoader() const { return textLoader; }
    TextFile GetTextFile(const std::string& filename);
    void PutStringClipped(int x, int y, const std::string& str, int maxLen);
private:
    int width = 0;
    int height = 0;
    TextLoader* textLoader = nullptr;

    std::vector<WCHAR> buffer;

    static constexpr WCHAR ClearChar = L' '; // 타입 일치
};
