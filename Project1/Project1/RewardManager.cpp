#include "RewardManager.h"
#include "BattleResult.h"
#include "Monster.h"
#include "Character.h"
#include <iostream>

RewardManager* RewardManager::instance = nullptr;

BattleResult RewardManager::generateRewards(const Monster* monster) {
    BattleResult result(0, 0);

    result.monsterName = monster->getName();
    result.isBossKill = monster->isBossMonster();

    if (monster->isBossMonster()) {
        // *임시* 몬스터의 공격력만큼 돈 벌기
        result.goldEarned = monster->getAttack();
        // *임시* 보스는 잡으면 1업하게끔
        result.expEarned = 100;
    } else {
        // *임시* 몬스터의 공격력만큼 돈 벌기
        result.goldEarned = monster->getAttack();
        // 우선 요구사항 따라서
        result.expEarned = 50;
    }

    return result;
}

void RewardManager::applyRewards(Character* player, const BattleResult& result) {
    player->addGold(result.goldEarned);
    player->addExperience(result.expEarned);
}

void RewardManager::displayRewards(const BattleResult& result) {
    std::cout << "\n획득 보상:\n";
    std::cout << "💰 골드: " << result.goldEarned << " G\n";
    std::cout << "⭐ 경험치: " << result.expEarned << " EXP\n";
}