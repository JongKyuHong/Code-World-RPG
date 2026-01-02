#include "EffectManager.h"
#include <iostream>
void EffectManager::apply(Character& c, const Effect& e, int sign) {
    switch (e.type) {
    case EffectType::HealFlat:
        c.heal(e.value);
        break;
    case EffectType::AddStatFlat: {
        const int delta = sign * e.value;
        switch (e.stat) {
        case StatType::Attack:    c.addAttack(delta); break;
        case StatType::MaxHealth: c.addMaxHealth(delta); break;
        default: break;
        }
        break;
    }
    }
}

void EffectManager::applyAll(Character& c,
    const std::vector<Effect>& effect,
    int sign) {
    for (const auto& e : effect)
        apply(c, e, sign);
}