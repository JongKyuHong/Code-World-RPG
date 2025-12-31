#pragma once

#include <string>
#include "Item.h"
#include "Entity.h"

class Monster : public Entity
{
protected:
    std::string mobInfo;
    bool isBoss;

public:
    Monster(const std::string& n, const std::string& info,
        int hp, int atk, bool boss = false) : Entity(n, hp, atk),
        mobInfo(info), isBoss(boss) {
    }

    virtual ~Monster() override {}

    std::string getMobInfo() {
        return mobInfo;
    };
    bool isBossMonster() const { return isBoss; }

    virtual void onDeath() = 0;
};
