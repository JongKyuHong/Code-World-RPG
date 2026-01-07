#include <iostream>
#include <algorithm>
#include "Inventory.h"
#include "Item.h"
#include "Character.h"
#include "EffectManager.h"
#include "EffectSystem.h"

static bool isInItems(const std::vector<Item*>& items, Item* p) {
    return std::find(items.begin(), items.end(), p) != items.end();
}

static int slotToIndex(EquipSlot slot) {
    return (slot == EquipSlot::Weapon) ? 0 : 1; // Weapon=0, Armor=1 정책 고정
}

Inventory::Inventory()
    : equipped(2, nullptr) // Weapon / Armor
{
}

Inventory::~Inventory() {
    // items 소유 해제
    for (auto* p : items) delete p;
    items.clear();

    // 장착 슬롯 소유 해제
    // (현재 정책상 equipped는 items에서 제거된 포인터를 들고 있으므로 중복 delete는 거의 없지만,
    //  안전 방어로 items에 있던 포인터면 건너뛰도록 함)
    for (auto*& p : equipped) {
        if (!p) continue;
        if (!isInItems(items, p)) { // items.clear() 이후라 항상 false지만, 방어 구조를 유지하려면 clear 이전에 체크해야 함
            delete p;
        }
        p = nullptr;
    }
}

void Inventory::add(Item* item) {
    if (!item) return;
    items.push_back(item);
}

void Inventory::listItems() const {
    std::cout << "=== Inventory ===\n";
    for (int i = 0; i < (int)items.size(); ++i) {
        std::cout << i << ") " << (items[i] ? items[i]->getName() : "(null)") << "\n";
    }
}

void Inventory::listEquipped() const {
    std::cout << "=== Equipped ===\n";
    std::cout << "Weapon: " << (equipped[0] ? equipped[0]->getName() : "(none)") << "\n";
    std::cout << "Armor : " << (equipped[1] ? equipped[1]->getName() : "(none)") << "\n";
}

bool Inventory::isEquippedPtr(Item* p) const {
    for (auto* e : equipped) {
        if (e == p) return true;
    }
    return false;
}

bool Inventory::isEquippedName(const std::string& itemName) const {
    for (auto* it : equipped) {
        if (it && it->getName() == itemName) return true;
    }
    return false;
}

bool Inventory::useItem(Character* c, int index, EffectSystem& es, EffectManager& manager, std::string* outUsedName)
{
    if (!c) return false;
    if (index < 0 || index >= (int)items.size()) return false;

    Item* it = items[index];
    if (!it) return false;

    if (isEquippedPtr(it)) {
        std::cout << "장착 중인 아이템은 사용할 수 없습니다.\n";
        return false;
    }
    if (it->isEquipItem()) {
        std::cout << "장비 아이템은 '사용'이 아니라 '장착'하세요.\n";
        return false;
    }

    if (outUsedName) *outUsedName = it->getName();

    es.applyFromItem(*c, it->getEffect(), manager);

    delete it;
    items.erase(items.begin() + index);
    return true;
}

void Inventory::equipItem(Character* c, int index, EquipSlot slot) {
    if (!c) return;
    if (index < 0 || index >= (int)items.size()) return;

    Item* newEq = items[index];
    if (!newEq) return;

    // 장비 아이템만 장착 가능
    if (!newEq->isEquipItem()) {
        std::cout << "이 아이템은 장비가 아닙니다.\n";
        return;
    }

    const int slotIdx = slotToIndex(slot);

    // 기존 장비가 있으면 효과 원복 후 인벤토리로 되돌림
    if (equipped[slotIdx]) {
        Item* old = equipped[slotIdx];
        EffectManager::applyAll(*c, old->getEffect(), -1);
        items.push_back(old);
        equipped[slotIdx] = nullptr;
    }

    // 새 장비를 인벤에서 빼서 장착 슬롯에 넣고 효과 적용
    equipped[slotIdx] = newEq;
    items.erase(items.begin() + index);
    EffectManager::applyAll(*c, newEq->getEffect(), +1);
}

void Inventory::unequipItem(Character* c, EquipSlot slot) {
    if (!c) return;

    const int slotIdx = slotToIndex(slot);
    if (!equipped[slotIdx]) return;

    Item* eq = equipped[slotIdx];

    // 장비 효과 원복 후 인벤토리로 이동
    EffectManager::applyAll(*c, eq->getEffect(), -1);
    items.push_back(eq);
    equipped[slotIdx] = nullptr;
}

int Inventory::findIndexByName(const std::string& name) const {
    for (int i = 0; i < (int)items.size(); ++i) {
        if (items[i] && items[i]->getName() == name) return i;
    }
    return -1;
}

Item* Inventory::getEquipped(EquipSlot slot) const {
    const int slotIdx = slotToIndex(slot);
    if (slotIdx < 0 || slotIdx >= (int)equipped.size()) return nullptr;
    return equipped[slotIdx];
}

Item* Inventory::extractAt(int index) {
    if (index < 0 || index >= (int)items.size()) return nullptr;

    Item* out = items[index];
    items.erase(items.begin() + index);
    return out;
}

int Inventory::size() const {
    return (int)items.size();
}
