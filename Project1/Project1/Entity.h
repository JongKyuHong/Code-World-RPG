#pragma once

#include <string>

class Entity
{
protected:
    std::string name;
    int health;
    int maxHealth;
    int attack;

public:
    Entity(const std::string& name, int health, int attack) :
        name(name), health(health), maxHealth(health), attack(attack) {}

    virtual ~Entity() {}

    const std::string& getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttack() const { return attack; }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }

    bool isAlive() const {
        return health > 0;
    }
};


