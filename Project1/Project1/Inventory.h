#pragma once
#include <vector>
#include <string>

#include "Types.h"

// 전방 선언(헤더 의존성 최소화)
class Item;
class Character;
class EffectSystem;
class EffectManager;

class Inventory {
private:
    std::vector<Item*> items;      // 인벤토리 아이템(소유)
    std::vector<Item*> equipped;   // 슬롯별 장비(소유) [0]=Weapon [1]=Armor

public:
    Inventory();
    ~Inventory();

    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    Inventory(Inventory&&) = delete;
    Inventory& operator=(Inventory&&) = delete;

    void add(Item* item);

    void listItems() const;
    void listEquipped() const;
    const std::vector<Item*>& getItems() const { return items; }

    // 소비/버프 사용 (구현은 cpp에서 Effect/Character 포함)
    bool useItem(Character* c, int index, EffectSystem& es, EffectManager& manager, std::string* outUsedName = nullptr);
    // 장비
    void equipItem(Character* c, int index, EquipSlot slot);
    void unequipItem(Character* c, EquipSlot slot);

    // 조회/유틸
    int findIndexByName(const std::string& name) const;
    Item* getEquipped(EquipSlot slot) const;          // 없으면 nullptr

    // 장착 판정
    bool isEquippedName(const std::string& itemName) const;
    bool isEquippedPtr(Item* p) const;

    // 소유권 이동/크기
    Item* extractAt(int index);
    int size() const;
};
