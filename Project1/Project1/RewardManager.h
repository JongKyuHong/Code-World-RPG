#pragma once

#include "BattleResult.h"
#include "Monster.h"
#include "Character.h"

class RewardManager
{
private:
    static RewardManager* instance;
    RewardManager() {}
public:
    static RewardManager* getInstance();
    // 몬스터이기고
    // 몬스터가 보스라면?? 정해진 아이템
    // 일반몹 일반아이템 랜덤, 골드 랜덤, 경험치 50
    BattleResult generateRewards(const Monster* monster);

    void applyRewards(Character* player, const BattleResult& result);
    void displayRewards(const BattleResult& result);
};
