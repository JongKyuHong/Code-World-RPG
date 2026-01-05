#pragma once

#include "Character.h"
#include "Monster.h"
#include "BattleResult.h"
#include "UIManager.h"

class BattleService
{
private:
    Character* player;
    Monster* monster;
    UIManager uiManager;

    // 전투로그 남기기
    std::vector<std::string> battleLog;

    void playerTurn();
    void monsterTurn();
    void displayBattleStatus();
    void addLog(const std::string& message);

public:
    // 1. 크리티컬 여부 결정 (선언)
    bool rollCritical();

    // 2. 크리티컬 데미지 적용 (선언)

    BattleService() : player(nullptr), monster(nullptr) {}
    BattleResult battle(Character* p, Monster* m);

    void displayLog();

    // 크리티컬 데미지 적용
    int applyCriticalMultiplier(int baseDamage, bool isCritical);
};


