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
    int maxHeight; // 최대체력
    int attack; // 공격력
    int experience; // 경험치
    int gold; // 골드

    std::vector<Item*> equipment; // 현재 장비한 아이템
    std::vector<Item*> Inventory; // 소비, 장비, 기타

public:
    Character(const std::string& name) : name(name) {}

    static Character* getInstance(const std::string& name = "") {};

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

