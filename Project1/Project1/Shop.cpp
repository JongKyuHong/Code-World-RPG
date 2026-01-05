#include <iostream>
#include "Shop.h"
#include "Item.h"
#include "ItemFactory.h"

Shop* Shop::instance = nullptr;

// 상점 품목
std::vector<Item*> Items;
static Shop* instance;

void initializeItem() {
    Items.push_back(ItemFactory::createHeal("Small Potion", 10, 50));
    Items.push_back(ItemFactory::createEquip("Iron Sword",20, EquipSlot::Weapon, /*effects*/));
    Items.push_back(ItemFactory::createBuff("Rage Scroll", 5, /*effects*/));
}

Shop* Shop::getInstance()
{
    if (instance == nullptr)
        instance = new Shop();
    return instance;
}

void Shop::displayItems()
{
    std::cout << "========================================\n";
    std::cout << "              ★ 상 점 ★               \n";
    std::cout << "========================================\n\n";
    for (int i = 0; i < (int)Items.size(); ++i)
    {
        Item* it = Items[i];
        if (!it) continue;

        std::cout << "  [" << i << "]  "
            << it->getName()
            << "    >>>    "
            << it->getPrice() << " G\n\n";
    }

    std::cout << "========================================\n\n";
}


Item* Shop::buyItem(Character* character, int index)
{
    if (!character) return nullptr;
    if (index < 0 || index >= (int)Items.size()) return nullptr;

    Item* it = Items[index];
    int price = it->getPrice();

    if (!character->spendGold(price)) {
        std::cout << "골드가 부족합니다.\n";
        return nullptr;
    }

    std::cout << "구매 완료!" << price << "G\n";
    return it;
}

void Shop::addItem(Item* item)
{
    if (item == nullptr) return;
    Items.push_back(item);
}

int Shop::sellItem(Character* character, Item* item)
{
    if (!character || !item) return 0;

    int sellPrice = item->getPrice() * 60 / 100;
    character->addGold(sellPrice);

    std::cout << "[" << item->getName() << "] 판매: +" << sellPrice << "G\n";
    return sellPrice;
    if (item == nullptr)
        return 0;

    int sellPrice = item->getPrice() * 60 / 100;
    return sellPrice;
}