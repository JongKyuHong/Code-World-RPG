#include "ItemFactory.h"
#include <utility>      // std::move
#include "HealItem.h"
#include "BuffItem.h"
#include "EquipItem.h"

Item* ItemFactory::createHeal(std::string name, int price, int healAmount) {
    return new HealItem(std::move(name), price, healAmount);
}

Item* ItemFactory::createBuff(std::string name, int price, std::vector<Effect> effect) {
    return new BuffItem(std::move(name), price, std::move(effect));
}

Item* ItemFactory::createEquip(std::string name, int price, EquipSlot slot, std::vector<Effect> equipEffect) {
    return new EquipItem(std::move(name), price, slot, std::move(equipEffect));
}

Item* ItemFactory::createWeapon(std::string name, int price, int attackPlus) {
    std::vector<Effect> effects{
        Effect{ EffectType::AddStatFlat, StatType::Attack, attackPlus, 0 }
    };
    return new EquipItem(std::move(name), price, EquipSlot::Weapon, std::move(effects));
}

Item* ItemFactory::createArmor(std::string name, int price, int maxHpPlus) {
    std::vector<Effect> effects{
        Effect{ EffectType::AddStatFlat, StatType::MaxHealth, maxHpPlus, 0 }
    };
    return new EquipItem(std::move(name), price, EquipSlot::Armor, std::move(effects));
}
