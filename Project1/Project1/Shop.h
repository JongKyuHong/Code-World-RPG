#pragma once

#include "Item.h"

#include <vector>

class Shop
{
private:
    // 상점 품목
    std::vector<Item*> Items;
    static Shop* instance;

    Shop() {
        intializeItem();
    }

    // 초기아이템 뭐넣을지 여기서 생성
    void intializeItem() {
        // Items.push_back(new HealthPotion("체력물약", 50));
    }

public:
    static Shop* getInstance() {
        if (instance == nullptr) {
            instance = new Shop();
        }
        return instance;
    };

    // 품목 보기
    void displayItems() {}

    // 구매하기
    Item* buyItem(Character* character, int index) {}

    // 아이템 추가하기
    void addItem(Character* character, Item* item) {}

    // 아이팀 구매하기, 돈 반환
    int sellItem(Item* item) {}
};

