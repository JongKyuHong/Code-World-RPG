#pragma once

#include <vector>
#include <string>
#include "Item.h"

class Character
{
private:
    static Character* instance; // ?깃????몄뒪?댁뒪

    std::string name; // 罹먮┃?곗씠由?
    int level; // ?덈꺼
    int health; // 泥대젰
    int maxHeight; // 理쒕?泥대젰
    int attack; // 怨듦꺽??
    int experience; // 寃쏀뿕移?
    int gold; // 怨⑤뱶

    std::vector<Item*> equipment; // ?꾩옱 ?λ퉬???꾩씠??
    std::vector<Item*> Inventory; // ?뚮퉬, ?λ퉬, 湲고?

public:
    Character(const std::string& name) : name(name) {}

    static Character* getInstance(const std::string& name = "") {};

    // ?곹깭 異쒕젰
    void displayStatus() {}

    // ?덈꺼??
    void levelUp() {}

    // ?꾩씠???ъ슜
    void useItem(int index) {}

    // ?λ퉬 ?꾩씠??李⑹슜
    void equipItem(int index) {}

    // ?λ퉬 ?꾩씠???댁젣
    void UnequipItem(int index) {}
};

