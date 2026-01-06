#pragma once

#include "Monster.h"
#include "Item.h"
#include <vector>

class VariableSlime : public Monster
{
private:
    std::string name;
    std::string mobInfo;
    int health;
    int attack;
    bool isBoss;

    int minGold;
    int maxGold;

    std::vector<Item*> dropItem;

public:
    VariableSlime(const std::string& name, const std::string& mobInfo, int health, 
        int attack, bool isBoss ) : name(name), mobInfo(mobInfo), health(health),
        attack(attack), isBoss(isBoss){
        // 드롭아이템 초기화
    }

    std::string getName() override {}
    std::string getMobInfo() override  {}
    int getHealth() override  {}
    int getAttack() override  {}
    void takeDamage(int damage) override {}

    // 드롭아이템
    Item* getDropItem() {};

    // 랜덤서써 10에서20
    int getGold() {};

    void setHealth() {};
};