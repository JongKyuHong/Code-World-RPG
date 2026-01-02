#pragma once
#include <vector>
#include <string>
#include <unordered_map>
struct TextFile {
public:
    int width() const {
        return textLines.empty() ? 0 : static_cast<int>(textLines[0].size());
    }

    int height() const {
        return static_cast<int>(textLines.size());
    }

    std::vector<std::wstring> textLines;
};
class TextLoader {
public:
    TextLoader() {
        ReadFiles();
    }

    TextFile GetTexts(const std::string& filename) {
        auto it = textFiles.find(filename);
        if (it != textFiles.end()) {
            return it->second;
        }
        return TextFile{};
    }
private:
    void ReadFiles();
    std::unordered_map<std::string, TextFile> textFiles;
};