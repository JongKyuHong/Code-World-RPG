#pragma once
#include <string>

class Inventory;
class MapAsciiArtRepository;

class SimpleDataLoader {
public:
    bool load(const std::string& path,
        Inventory& inv,
        MapAsciiArtRepository& artRepo) const;
};