#pragma once

#include "Item.h"
#include <string>

class KeyBoard : Item
{
private:
    // 이름
    std::string name;

    // 공격력
    int attackIncrease;

    // 아이템 가치
    int gold;

public:
    KeyBoard() {};

    // 이름반환
    std::string getName() override {}

    // 장착 + 스텟영구적으로 올라감
    void use(Character* character) override {}

    // 해제
    void Unequip(Character* character) {}
};


