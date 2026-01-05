#include "TextLoader.h"
#include <windows.h>
#include <fstream>
#include <codecvt>
#include <locale>
#include <algorithm>

void TextLoader::ReadFiles()
{
    textFiles.clear();

    ReadFilesRecursive(L"./Texts");
}

void TextLoader::ReadFilesRecursive(const std::wstring& directory)
{
    std::wstring searchPath = directory + L"\\*";

    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do
    {
        std::wstring fileNameW = findData.cFileName;

        if (fileNameW == L"." || fileNameW == L"..")
            continue;

        std::wstring fullPath = directory + L"/" + fileNameW;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            ReadFilesRecursive(fullPath);
            continue;
        }

        if (fileNameW.size() < 4 ||
            fileNameW.substr(fileNameW.size() - 4) != L".txt")
            continue;

        std::wifstream file(fullPath);
        if (!file.is_open())
            continue;

        file.imbue(std::locale(
            std::locale(),
            new std::codecvt_utf8<wchar_t>
        ));

        TextFile textFile;
        std::wstring line;

        while (std::getline(file, line))
        {
            textFile.textLines.push_back(line);
        }

        std::wstring relativePath = fullPath.substr(8);

        std::string key(relativePath.begin(), relativePath.end());
        std::replace(key.begin(), key.end(), '\\', '/');
        textFiles.emplace(key, std::move(textFile));

    } while (FindNextFileW(hFind, &findData));

    FindClose(hFind);
}
