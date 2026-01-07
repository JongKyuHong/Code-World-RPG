#include "EquipItem.h"
EquipItem::EquipItem(std::string name,
    int price,
    EquipSlot slot,
    std::vector<Effect> effect)
    : Item(std::move(name), price, std::move(effect)),
    slot(slot)
{
}
EquipSlot EquipItem::getSlot() const { 
    return slot; 
}

Item* EquipItem::clone() const {
    return new EquipItem(name, price, slot, effect);
}