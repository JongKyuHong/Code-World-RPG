#include <iostream>
#include "Shop.h"
#include "ItemFactory.h"
#include "Types.h"
#include "Character.h"
#include "Inventory.h"

Shop* Shop::instance = nullptr;

Shop::Shop() {
    initializeItem();
}

Shop::~Shop() {
    // 상점이 Items를 소유한다는 정책이면 여기서 정리
    for (auto* p : Items) delete p;
    Items.clear();
}

void Shop::initializeItem() {
    Items.push_back(ItemFactory::createHeal("Small Potion", 10, 50));
    Items.push_back(ItemFactory::createWeapon("One Hand Sword", 50, 10));
    Items.push_back(
        ItemFactory::createBuff(
            "최대 체력 버프(+50 / 3턴)",
            200,
            std::vector<Effect>{
        Effect{ EffectType::AddStatFlat, StatType::MaxHealth, 50, 3 }
    }
        )
    );
}

Shop* Shop::getInstance() {
    if (instance == nullptr)
        instance = new Shop();
    return instance;
}

void Shop::displayItems() {
    std::cout << "========================================\n";
    std::cout << "              ★ 상 점 ★               \n";
    std::cout << "========================================\n\n";

    for (int i = 0; i < (int)Items.size(); ++i) {
        Item* it = Items[i];
        if (!it) continue;

        std::cout << "  [" << i << "]  "
            << it->getName()
            << "    >>>    "
            << it->getPrice() << " G\n\n";
    }
    std::cout << "========================================\n\n";
}

Item* Shop::buyItem(Character* character, int index) {
    if (!character) return nullptr;
    if (index < 0 || index >= (int)Items.size()) return nullptr;

    Item* it = Items[index];
    if (!it) return nullptr;

    int price = it->getPrice();
    // 결제는 Character에게 위임
    if (!character->spendGold(it->getPrice())) {
        // 실패 메시지는 spendGold 내부에서 이미 출력됨
        return nullptr;
    }

    // ⚠️ 매우 중요:
    // 지금은 "상점이 소유한 같은 포인터"를 넘기면 위험함.
    // 최소 수정 버전에서는 "소유권 이전" 방식으로 처리:
    Item* boughtItem = Items[index]->clone();
    Items[index] = nullptr; // 상점에서 비워둠(재고 감소)

    std::cout << "구매 완료! -" << price << "G\n";
    return boughtItem; // 이제 이 포인터는 플레이어/인벤토리가 소유
}

void Shop::addItem(Item* item) {
    if (!item) return;
    Items.push_back(item);
}

int Shop::sellItem(Character* character, Item* item) {
    if (!character || !item) return 0;

    int sellPrice = item->getPrice() * 60 / 100;
    character->addGold(sellPrice);

    std::cout << "[" << item->getName() << "] 판매: +" << sellPrice << "G\n";

    // ⚠️ 여기서 item delete 할지 말지는 "인벤토리 정책"에 따라 결정해야 함.
    // 보통은 인벤토리에서 제거하면서 delete까지 인벤토리가 책임짐.
    return sellPrice;
}

void Shop::clearItems() {
    for (auto* p : Items) delete p;
    Items.clear();
}
