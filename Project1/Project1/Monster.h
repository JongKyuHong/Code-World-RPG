#pragma once

#include <string>
#include "Item.h"
#include "Entity.h"

class Monster : public Entity
{
protected:
    std::string mobInfo;
    bool isBoss;
    int phase = 1;

public:
    Monster(const std::string& n, const std::string& info,
        int hp, int atk, int phaseNum = 1, bool boss = false) : Entity(n, hp, atk),
        mobInfo(info), phase(phaseNum), isBoss(boss) {
    }

    virtual ~Monster() override {}

    std::string getMobInfo() {
        return mobInfo;
    };
    bool isBossMonster() const { return isBoss; }
    int GetPhase() const { return phase; }
    virtual void onDeath() = 0;
};

