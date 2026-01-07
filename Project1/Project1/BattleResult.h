#pragma once

#include <string>
#include <vector>

struct BattleResult {
    bool playerWon;          // 승리 여부
    int goldEarned;          // 획득 골드
    int expEarned;           // 획득 경험치
    int levelUps;            // 레벨업 수치
    std::string monsterName; // 처치한 몬스터 이름
    bool isBossKill;         // 보스 처치 여부
    int totalDamageDealt;    // 플레이어가 준 총 데미지
    int totalDamageTaken;    // 플레이어가 받은 총 데미지
    int turnCount;           // 총 턴 수
    // 추가: 드랍된 아이템 이름 목록
    std::vector<std::string> droppedItemNames;

    BattleResult(int gold, int exp)
        : playerWon(false), goldEarned(gold), expEarned(exp), levelUps(0), monsterName(""), isBossKill(false),
        totalDamageDealt(0), totalDamageTaken(0), turnCount(0) {
    }
};