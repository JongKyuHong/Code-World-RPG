#include "BattleService.h" // 내 설계도를 가져옵니다.


#include <cstdlib>         // rand() 함수를 위해 필요합니다.

// 1. 크리티컬 여부 결정 (정의)
// BattleService:: 를 붙여서 "BattleService 소속 함수다"라고 알려줍니다.

bool BattleService::rollCritical()
{
    // 0~9 사이의 숫자 중 0, 1, 2가 나오면 크리티컬 (30%)
    if (rand() % 10 < 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 2. 크리티컬 데미지 적용 (정의)
int BattleService::applyCriticalMultiplier(int baseDamage, bool isCritical)
{
    if (isCritical == true)
    {
        // 크리티컬이면 2배로 뻥튀기
        return baseDamage * 2;
    }
    else
    {
        // 아니면 원래 데미지 그대로
        return baseDamage;
    }
}