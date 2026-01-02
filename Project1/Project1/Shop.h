#pragma once

#include "Item.h"
#include "Character.h"
#include <vector>



class Shop
{
private:
    // 상점 품목
    std::vector<Item*> Items;
    static Shop* instance;

    Shop();

    // 초기아이템 뭐넣을지 여기서 생성
    void initializeItem();

public:
    static Shop* getInstance();

    // 품목 보기
    void displayItems() {}

    // 구매하기
    Item* buyItem(Character* character, int index) {}

    // 아이템 추가하기
    void addItem(Item* item) {}

    // 아이팀 구매하기, 돈 반환
    int sellItem(Character* character, Item* item) {}
};

