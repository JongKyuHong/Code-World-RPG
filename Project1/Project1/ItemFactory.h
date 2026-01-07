#pragma once

#include <string>
#include <vector>
#include "Types.h"   // Effect, EffectType, StatType, EquipSlot 등
#include "Item.h"    // Item* 반환하려면 선언 필요(혹은 forward 선언 + include는 cpp에서)


class ItemFactory {
public:
    // 회복 아이템 생성
    static Item* createHeal(std::string name, int price, int healAmount);

    // 버프 아이템 생성
    static Item* createBuff(std::string name, int price, std::vector<Effect> effect);

    // 장비 일반 생성(Effect 리스트 직접)
    static Item* createEquip(std::string name, int price, EquipSlot slot, std::vector<Effect> equipEffect);

    // 장비 편의 생성: 공격력만
    static Item* createWeapon(std::string name, int price, int attackPlus);

    // 장비 편의 생성: 최대체력만
    static Item* createArmor(std::string name, int price, int maxHpPlus);

          
};