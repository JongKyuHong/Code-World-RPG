#pragma once

#include "Character.h"
#include "Monster.h"
#include "BattleResult.h"

class BattleService
{
private:
    Character* player;
    Monster* monster;

    // 전투로그 남기기
    std::vector<std::string> battleLog;

    void playerTurn();
    void monsterTurn();
    void displayBattleStatus();
    void addLog(const std::string& message);

public:

    BattleService() : player(nullptr), monster(nullptr) {}
    BattleResult battle(Character* p, Monster* m);

    void displayLog();

    // 크리티컬 확률
    bool rollCritical();

    // 크리티컬 데미지 적용
    int applyCriticalMultiplier(int baseDamage, bool isCritical);
};

