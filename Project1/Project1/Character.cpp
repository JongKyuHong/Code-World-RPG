#include "Character.h"
Character* Character::instance = nullptr;

void Character::levelUp()
{
    // 만렙일시 레벨업 불가
    if (level >= 10)
    {
        experience = 0;
        return;
    }

    // 레벨업
    if (experience >= 100)
    {
        level += 1;
        experience = 0;

        // 레벨업시 체력회복
        health = 100;
    }
}