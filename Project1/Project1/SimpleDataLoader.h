// SimpleDataLoader.h
#pragma once
#include <string>
#include <vector>

class Inventory;
class MapAsciiArtRepository;
class Item;
class RewardRepository;

class SimpleDataLoader {
public:
    // 기존: 인벤에 직접 생성해서 add
    bool load(const std::string& path,
        Inventory& inv,
        MapAsciiArtRepository& artRepo) const;

    // 추가: 생성된 아이템들을 vector로 수집 (Shop/DropTable 초기화용)
    bool loadToVector(const std::string& path,
        std::vector<Item*>& outItems,
        MapAsciiArtRepository& artRepo) const;

    bool loadRewards(
        const std::string& path,
        RewardRepository& rewards
    )const ;

private:
    // 내부 공통 구현
    template <typename ItemConsumer>
    bool loadImpl(const std::string& path,
        ItemConsumer&& consumeItem,
        MapAsciiArtRepository& artRepo) const;
};
