#pragma once

#include <vector>
#include <string>
#include "Item.h"

class Character
{
private:
    static Character* instance; // 싱글톤 인스턴스

    std::string name; // 캐릭터이름
    int level; // 레벨
    int health; // 체력
    int maxHealth; // 최대체력
    int attack; // 공격력
    int experience; // 경험치
    int gold; // 골드

    std::vector<Item*> equipment; // 현재 장비한 아이템
    std::vector<Item*> inventory; // 소비, 장비, 기타

    Character(const std::string& n)
        : name(n), level(1), health(200), maxHealth(200),
        attack(30), experience(0), gold(0) {
    }

    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;

public:
    ~Character() {
        for (auto item : equipment) {
            delete item;
        }
        for (auto item : inventory) {
            delete item;
        }
    }
    static Character* getInstance(const std::string& name = "") {
        if (instance == nullptr) {
            instance = new Character(name);
        }
        return instance;
    };

    static void destroyInstance() {
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
        }
    }

    std::string getName() const { return name; }
    int getLevel() const { return level; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttack() const { return attack; }
    int getExperience() const { return experience; }
    int getGold() const { return gold; }

    // 상태 출력
    void displayStatus() {}

    // 레벨업
    void levelUp() {}

    // 아이템 사용
    void useItem(int index) {}

    // 장비 아이템 착용
    void equipItem(int index) {}

    // 장비 아이템 해제
    void UnequipItem(int index) {}
};
