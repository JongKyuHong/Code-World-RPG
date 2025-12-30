#pragma once

#include <string>
#include "Item.h"

class Monster
{
public:
    virtual std::string getName() = 0;
    virtual std::string getMobInfo() = 0;
    virtual int getHealth() = 0;
    virtual int getAttack() = 0;
    virtual void takeDamage(int damage) = 0;
    virtual ~Monster() {}
};
