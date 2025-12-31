#pragma once

#include <vector>
#include <string>
#include "Entity.h"
#include "Item.h"

class Character : public Entity {
private:
    static Character* instance; // 싱글톤 인스턴스

    // 레벨
    int level; 
    // 경험치
    int experience;
    // 골드
    int gold;

    std::vector<Item*> equipment; // 현재 장비한 아이템
    std::vector<Item*> inventory; // 소비, 장비, 기타

    Character(const std::string& n)
        : Entity(n, 200, 30)
    {
        level = 1;
        experience = 0;
        gold = 0;
    }

    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;

public:
    ~Character() override {
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

    int getLevel() const { return level; }
    int getExperience() const { return experience; }
    int getGold() const { return gold; }

    // 상태 출력
    void displayStatus() {}

    // 경험치, 몬스터 1마리당 1업이면 제외하기
    void addExperience(int value) {
        experience += value;
    }

    // 레벨업
    void levelUp() {}

    // 골드 획득
    void addGold(int value) { 
        gold += value; 
    }

    // 아이템 획득
    void addItem(Item* item) {
        inventory.push_back(item);
    }

    // 아이템 사용
    void useItem(int index) {}

    // 장비 아이템 착용
    void equipItem(int index) {}

    // 장비 아이템 해제
    void UnequipItem(int index) {}

    // 버프 물약 사용시 공격력
    void setAttack(int newAttack) { 
        attack = newAttack;
    }
};