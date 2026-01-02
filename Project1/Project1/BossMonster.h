#pragma once

#include "Monster.h"
#include <string>
class BossMonster : public Monster {
public:
    BossMonster(const std::string& n,
        const std::string& info,
        int hp,
        int atk)
        : Monster(n, info, hp, atk, true) {
    }

    void onDeath() override {
        // 보스 몬스터가 죽었을 때 처리
        // 보스 클리어 연출, 특수 보상 등
    }
};
