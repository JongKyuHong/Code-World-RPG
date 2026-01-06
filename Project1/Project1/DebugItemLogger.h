#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Item.h"
#include "EquipItem.h"
#include "HealItem.h"
#include "BuffItem.h"
#include "Types.h"   // Effect, EffectType, StatType, EquipSlot

namespace DebugItemLogger {

    // 臾몄옄??蹂???ы띁??(??enum??留욊쾶 耳?댁뒪 異붽?)
    inline const char* toString(EquipSlot s) {
        switch (s) {
        case EquipSlot::Weapon: return "Weapon";
        case EquipSlot::Armor:  return "Armor";
        default:                return "UnknownSlot";
        }
    }

    inline const char* toString(EffectType t) {
        switch (t) {
        case EffectType::AddStatFlat: return "AddStatFlat";
            // ?꾩슂?섎㈃ 異붽?:
            // case EffectType::Heal: return "Heal";
        default: return "UnknownEffectType";
        }
    }

    inline const char* toString(StatType s) {
        switch (s) {
        case StatType::Attack:     return "Attack";
        case StatType::MaxHealth:  return "MaxHealth";
            // ?꾩슂?섎㈃ 異붽?:
            // case StatType::Gold: return "Gold";
        default: return "UnknownStatType";
        }
    }

    inline void printEffects(const std::vector<Effect>& effects) {
        std::cout << "  effects.size = " << effects.size() << "\n";
        for (size_t i = 0; i < effects.size(); ++i) {
            const auto& e = effects[i];
            std::cout << "   - [" << i << "] "
                << "type=" << toString(e.type)
                << ", stat=" << toString(e.stat)
                << ", value=" << e.value
                << ", duration=" << e.duration
                << "\n";
        }
    }

    // ?듭떖: Item* ?섎굹瑜??먯꽭??異쒕젰
    inline void dumpItem(const Item* item, const char* tag = "") {
        std::cout << "\n[ItemDump]" << (tag && tag[0] ? std::string(" ") + tag : "") << "\n";

        if (!item) {
            std::cout << "  (null item)\n";
            return;
        }

        // 怨듯넻: ?대쫫
        std::cout << "  name = " << item->getName() << "\n";

        // 1) EquipItem ?몄? ?뺤씤
        if (auto eq = dynamic_cast<const EquipItem*>(item)) {
            std::cout << "  kind = EquipItem\n";
            std::cout << "  slot = " << toString(eq->getSlot()) << "\n";
            printEffects(eq->getEffect());
            return;
        }

        // 2) HealItem ?몄? ?뺤씤
        if (auto heal = dynamic_cast<const HealItem*>(item)) {
            std::cout << "  kind = HealItem\n";
            // HealItem??getter媛 ?덉쑝硫?異쒕젰(?놁쑝硫???以???젣)
            // std::cout << "  healAmount = " << heal->getHealAmount() << "\n";
            // std::cout << "  maxStack   = " << heal->getMaxStack() << "\n";
            printEffects(heal->getEffect()); // HealItem??getEffect() ?쒓났?쒕떎??媛??
            return;
        }

        // 3) BuffItem ?몄? ?뺤씤
        if (auto buff = dynamic_cast<const BuffItem*>(item)) {
            std::cout << "  kind = BuffItem\n";
            printEffects(buff->getEffect());
            return;
        }

        // 4) 洹??? 湲곕낯 Item濡?泥섎━
        std::cout << "  kind = (unknown derived type)\n";
        printEffects(item->getEffect());
    }

} // namespace DebugItemLogger
