#include "HealItem.h"

// 단일 힐량 생성자
HealItem::HealItem(std::string name, int price, int healAmount)
    : Item(
        std::move(name),
        price,
        std::vector<Effect>{
    Effect{ EffectType::HealFlat, StatType::Health, healAmount, 0 }
}
)
{
}

// effect 직접 주입 생성자
HealItem::HealItem(std::string name, int price, std::vector<Effect> effect)
    : Item(std::move(name), price, std::move(effect))
{
}

Item* HealItem::clone() const {
    return new HealItem(name, price, effect);
}