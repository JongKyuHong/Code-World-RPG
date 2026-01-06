#pragma once

#include <Windows.h>   // WCHAR 정의
#include <vector>
#include <string>

struct TextFile;
class TextLoader;

class Renderer {
public:
    // 색상 enum
    enum Color {
        BLACK = 0,
        DARK_BLUE = 1,
        DARK_GREEN = 2,
        DARK_CYAN = 3,
        DARK_RED = 4,
        DARK_MAGENTA = 5,
        DARK_YELLOW = 6,
        LIGHT_GRAY = 7,
        DARK_GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        LIGHT_MAGENTA = 13,
        LIGHT_YELLOW = 14,
        WHITE = 15
    };

    Renderer(int w, int h);

    void Present(); // Buffer에 있는 것을 화면에 출력

    void Put(int x, int y, WCHAR c);                    // 특정 위치에 문자 그리기
    void PutString(int x, int y, const std::string&);  // 특정 위치에 문자열 그리기
    void PutBox(int x, int y, int w, int h);           // 특정 위치에 박스 그리기
    void PutTextFile(int x, int y, const std::string& filename); // 특정 위치에 텍스트 파일 그리기

    // 색상 기능 (수정: 색상 정보를 버퍼에 저장)
    void PutString(int x, int y, const std::string& str, Color color);
    void SetColor(Color foreground, Color background = BLACK);
    void ResetColor();

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
    std::vector<Color> colorBuffer;  // ✅ 색상 버퍼 추가
    Color currentColor = LIGHT_GRAY; // ✅ 현재 색상 저장

    static constexpr WCHAR ClearChar = L' ';
};
