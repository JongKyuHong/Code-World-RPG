#pragma once
#include "AsciiArtRepository.h"
#include <unordered_map>

// name -> AsciiLines 를 메모리에 들고 있는 단순 구현체.
// JSON을 파싱해서 이 맵을 채우는 쪽은 별도(Loader)에서 하면 됨.
class MapAsciiArtRepository final : public IAsciiArtRepository {
public:
    MapAsciiArtRepository() = default;

    void upsert(const std::string& itemName, AsciiLines lines) {
        arts_[itemName] = std::move(lines);
    }

    const AsciiLines* findByItemName(const std::string& itemName) const override {
        auto it = arts_.find(itemName);
        if (it == arts_.end()) return nullptr;
        return &it->second;
    }

private:
    std::unordered_map<std::string, AsciiLines> arts_;
};