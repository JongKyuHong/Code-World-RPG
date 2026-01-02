#pragma once

// 전투에 관련된 계산 (설계도)
class BattleService
{
public:
    // 1. 크리티컬 여부 결정 (선언)
    bool rollCritical();

    // 2. 크리티컬 데미지 적용 (선언)
    int applyCriticalMultiplier(int baseDamage, bool isCritical);
};

