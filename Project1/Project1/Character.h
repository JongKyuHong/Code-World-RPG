#pragma once

#include <vector>
#include <string>
#include "Entity.h"
#include "iostream"
class Item; // Item 클래스 전방 선언

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
    ~Character() = default;

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

    // 경험치, 일반 몬스터 50, 보스몬스터 100
    void addExperience(int value) {
        if (value <= 0) return;
        experience += value;
    }

    // 레벨업
    int levelUp() {
        static constexpr int MAX_LEVEL = 10;
        static constexpr int EXP_PER_LEVEL = 100;

        int levelUps = 0; //레벨업 수치


        while (experience >= EXP_PER_LEVEL && level < MAX_LEVEL) {
            experience -= EXP_PER_LEVEL;
            level += 1;
            levelUps += 1;

            // 네 규칙: Lv * 20 / Lv * 5 (레벨업 "후" 레벨 기준)
            maxHealth += level * 20;
            attack += level * 5;
            health = maxHealth;
        }

        // 레벨 캡이면 경험치 정책 (원하는 대로)
        if (level >= MAX_LEVEL) {
            level = MAX_LEVEL;
             experience = 0;
        }
        return levelUps;
    }
    bool spendGold(int amount)
    {
        if (amount <= 0) {
            std::cout << "[오류] 잘못된 결제 금액\n";
            return false;
        }

        if (gold < amount) {
            std::cout << "골드가 부족합니다. (보유: "
                << gold << "G)\n";
            return false;
        }

        gold -= amount;
        return true;
    }
    // 골드 획득
    void addGold(int value) {
        gold += value;
    }

    // 버프 물약 사용시 공격력
    void setAttack(int newAttack) {
        attack = newAttack;
    }

    void heal(int healAmount) {
        health += healAmount;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    void addAttack(int delta) {
        attack += delta;
    };

    void addMaxHealth(int delta) {
        maxHealth += delta;
    }

};
