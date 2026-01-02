#pragma once

#include <string>

struct BattleResult {
    bool playerWon;          // 승리 여부
    int goldEarned;          // 획득 골드
    int expEarned;           // 획득 경험치
    std::string monsterName; // 처치한 몬스터 이름
    bool isBossKill;         // 보스 처치 여부
    int totalDamageDealt;    // 플레이어가 준 총 데미지
    int totalDamageTaken;    // 플레이어가 받은 총 데미지
    int turnCount;           // 총 턴 수

    BattleResult()
        : playerWon(false), goldEarned(0), expEarned(0),
        monsterName(""), isBossKill(false),
        totalDamageDealt(0), totalDamageTaken(0), turnCount(0) {
    }
};
