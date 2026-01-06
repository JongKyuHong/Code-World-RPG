#pragma once

#include "Inventory.h"
#include "MapAsciiArtRepository.h"
#include "Shop.h"
#include "DropTable.h"
#include "RewardRepository.h"   // ✅ 추가

struct ItemContext {
    Shop* shop = nullptr;              // singleton or injected
    Inventory inventory;               // 아이템 소유
    DropTable dropTable;               // prototype 소유
    RewardRepository rewards;          // ✅ 추가: 몬스터 드랍 룰 저장소
    MapAsciiArtRepository artRepo;      // ASCII 리소스

    ItemContext() = default;

    ItemContext(const ItemContext&) = delete;
    ItemContext& operator=(const ItemContext&) = delete;
};