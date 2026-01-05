// Inventory.h
#pragma once
#include <vector>

#include<string>
#include "Types.h"
#include "Item.h"
class EffectSystem;
class EffectManager;
class Character;

class Inventory {
private:
    std::vector<Item*> items;      // 인벤토리 아이템(소유)
    std::vector<Item*> equipped;   // 슬롯별 장비(소유) [0]=Weapon [1]=Armor

public:
    Inventory();
    ~Inventory();

    // 복사 금지(포인터 소유하므로 깊은 복사/이중 delete 방지)
    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    // 이동은 필요하면 나중에 구현
    Inventory(Inventory&&) = delete;
    Inventory& operator=(Inventory&&) = delete;

    void add(Item* item);

    void listItems() const;
    void listEquipped() const;
    const std::vector<Item*>& getItems() const { return items; }

    void useItem(Character* c, int index, EffectSystem& es, EffectManager& mgr);                 // 소비/버프 사용
    void equipItem(Character* c, int index, EquipSlot slot); // 장비 착용
    void unequipItem(Character* c, EquipSlot slot);        // 장비 해제
    int findIndexByName(const std::string& name) const;
    // 장착 표시용(가능하면)
    Item* getEquipped(EquipSlot slot) const;          // 없으면 nullptr

    bool isEquippedName(const std::string& itemName) const;

    int size() const;
};

