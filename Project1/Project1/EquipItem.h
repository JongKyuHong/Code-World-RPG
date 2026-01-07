// EquipItem.h
#pragma once
#include "Item.h"
#include "Types.h"
#include <utility>

class EquipItem : public Item {
private:
    EquipSlot slot;

public:
    EquipItem(std::string name,
        int price,
        EquipSlot slot,
        std::vector<Effect> effect);
    EquipSlot getSlot() const;
    Item* clone() const override;
};