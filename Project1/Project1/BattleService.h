#pragma once

// 전투에 관련된 계산
class BattleService
{
public:
    // 크리티컬 확률
    bool rollCritical() {}

    // 크리티컬 데미지 적용
    int applyCriticalMultiplier(int baseDamage, bool isCritical) {};
};

