#pragma once

#include <string>
#include "Character.h"

class BuffPotion : public Item
{
private:
    // 이름
    std::string name;

    // 효과
    int effect;

    // 아이템 가치
    int gold;
public:
    BuffPotion() {}
    std::string getName() override {}
    void use(Character* character) override {}
};
