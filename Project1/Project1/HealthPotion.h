#pragma once

#include <string>
#include "Character.h"

class HealthPotion : public Item
{
private:
    // 이름
    std::string name;

    // 회복량
    int healthRestore;

    // 아이템 가치
    int gold;

public:
    HealthPotion() {}
    std::string getName() override {}
    void use(Character* character) override {}
};

