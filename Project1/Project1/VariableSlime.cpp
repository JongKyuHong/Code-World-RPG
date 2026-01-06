#include "VariableSlime.h"


//반환 함수들
std::string VariableSlime::getName()
{
    return name;
}

std::string VariableSlime::getMobInfo()
{
    return mobInfo;
}

int VariableSlime::getHealth()
{
    return health;
}

int VariableSlime::getAttack()
{
    return attack;
}

//몬스터 데미지 처리
void VariableSlime::takeDamage(int damage)
{
    health -= damage;

    if (health < 0)
        health = 0;
}
