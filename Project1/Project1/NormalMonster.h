#pragma once

#include "Monster.h"
#include <string>

class NormalMonster : public Monster
{
public:
    NormalMonster(const std::string& n,
        const std::string& info,
        int hp,
        int atk, int phaseNum = 1)
        : Monster(n, info, hp, atk, phaseNum, false) {
    }

    void onDeath() {
        // 노말몹 죽었을때 (드랍, 경험치 지급 등)
    }
};


