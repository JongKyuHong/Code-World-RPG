#pragma once
#include <vector>
#include "Item.h"
#include "Character.h"

class Shop {
private:
    std::vector<Item*> Items;    // 상점 품목(상점이 소유한다고 가정)
    static Shop* instance;

    Shop();
    void initializeItem();

public:
    ~Shop();

    static Shop* getInstance();

    void displayItems();
    Item* buyItem(Character* character, int index);
    void addItem(Item* item);
    int sellItem(Character* character, Item* item);
    int size() const { return (int)Items.size(); }
    Item* peek(int index) const {
        if (index < 0 || index >= (int)Items.size()) return nullptr;
        return Items[index];
    }
    void clearItems();
};