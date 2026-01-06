#pragma once
#include <string>
#include <vector>

using AsciiLines = std::vector<std::string>;

class IAsciiArtRepository {
public:
    virtual ~IAsciiArtRepository() = default;

    virtual const AsciiLines* findByItemName(const std::string& itemName) const = 0;
};
