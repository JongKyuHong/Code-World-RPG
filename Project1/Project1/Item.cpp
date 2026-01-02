#include "Item.h"

Item::Item(std::string name,int price, std::vector<Effect> effect) :
	name(std::move(name)), price(price), effect(std::move(effect)) {
}

std::string Item::getName() const {
	return name;
}
int Item::getPrice() const {
	return price;
}
const std::vector<Effect>& Item:: getEffect() const { return effect; }

bool Item::isHealItem() const {
    for (auto& e : effect)
        if (e.type == EffectType::HealFlat) return true;
    return false;
}
bool Item::isBuffItem() const {
    for (auto& e : effect)
        if (e.type == EffectType::AddStatFlat && e.duration > 0) return true;
    return false;
}
bool Item::isEquipItem() const {
    for (auto& e : effect)
        if (e.type == EffectType::AddStatFlat && e.duration == 0 &&
            (e.stat == StatType::Attack || e.stat == StatType::MaxHealth))
            return true;
    return false;
}
EquipSlot Item::getEquipSlotHint() const {
    for (auto& e : effect) {
        if (e.type == EffectType::AddStatFlat && e.duration == 0) {
            if (e.stat == StatType::Attack) return EquipSlot::Weapon;
            if (e.stat == StatType::MaxHealth) return EquipSlot::Armor;
        }
    }
    return EquipSlot::Weapon; // 기본값(장비가 아닐 땐 쓰지 않게 UI에서 체크)
}
