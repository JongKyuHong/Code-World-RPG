#include "TextLoader.h"
#include <windows.h>
#include <fstream>
#include <codecvt>
#include <locale>

void TextLoader::ReadFiles()
{
    textFiles.clear();

    std::wstring directory = L"./Texts\\*";

    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(directory.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do
    {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        std::wstring fileNameW = findData.cFileName;

        if (fileNameW.size() < 4 ||
            fileNameW.substr(fileNameW.size() - 4) != L".txt")
            continue;

        std::wstring fullPath = L"./Texts/" + fileNameW;

        std::wifstream file(fullPath);
        if (!file.is_open())
            continue;

        file.imbue(std::locale(
            std::locale(), // ✅ empty() 대신 기본 locale
            new std::codecvt_utf8_utf16<wchar_t> // ✅ Windows wchar_t(UTF-16) 대응
        ));

        TextFile textFile;
        std::wstring line;

        while (std::getline(file, line))
        {
            textFile.textLines.push_back(line);
        }

        std::string key(fileNameW.begin(), fileNameW.end());
        textFiles.emplace(key, std::move(textFile));

    } while (FindNextFileW(hFind, &findData));

    FindClose(hFind);
}