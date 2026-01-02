#include <iostream>
#include "Inventory.h"
#include "Item.h"
#include "Character.h"
#include "EffectManager.h"
#include "EffectSystem.h"

Inventory::Inventory()
    : equipped(2, nullptr) // 장비 슬롯 2칸(Weapon/Armor)을 nullptr로 초기화
{
}

Inventory::~Inventory() {
    // 인벤토리 소유 아이템 메모리 해제
    for (auto* p : items) delete p;
    items.clear();

    // 장착 슬롯에 남아있는 아이템도 해제(장비가 items에 없을 수 있음)
    for (auto*& p : equipped) {
        if (p) { delete p; p = nullptr; }
    }
}

void Inventory::add(Item* item) {
    if (!item) return;      // nullptr 방어
    items.push_back(item);  // 인벤토리가 아이템 소유권을 가짐
}

void Inventory::listItems() const {
    std::cout << "=== Inventory ===\n";
    // 인벤토리 목록(인덱스 기반) 출력
    for (int i = 0; i < (int)items.size(); ++i) {
        std::cout << i << ") " << items[i]->getName() << "\n";
    }
}

void Inventory::listEquipped() const {
    std::cout << "=== Equipped ===\n";
    // 장착 상태 출력(없으면 none)
    std::cout << "Weapon: " << (equipped[0] ? equipped[0]->getName() : "(none)") << "\n";
    std::cout << "Armor : " << (equipped[1] ? equipped[1]->getName() : "(none)") << "\n";
}

void Inventory::useItem(Character* c, int index, EffectSystem& es, EffectManager& manager) {
    if (!c) return; // 캐릭터 없으면 처리 불가
    if (index < 0 || index >= (int)items.size()) return; // 인덱스 범위 체크

    Item* item = items[index];

    // 아이템 효과를 규칙(EffectSystem)에 따라 적용(지속효과는 manager에 등록될 수 있음)
    es.applyFromItem(*c, item->getEffect(), manager);

    // 소비 아이템 정책: 사용 즉시 삭제 + 목록에서 제거
    delete item;
    items.erase(items.begin() + index);
}

void Inventory::equipItem(Character* c, int index, EquipSlot slot) {
    if (!c) return; // 캐릭터 없으면 처리 불가
    if (index < 0 || index >= (int)items.size()) return; // 인덱스 범위 체크

    const int slotIdx = (slot == EquipSlot::Weapon) ? 0 : 1; // 슬롯 enum → 배열 인덱스
    Item* newEq = items[index];

    // 기존 장비가 있으면 효과 원복 후 인벤토리로 되돌림
    if (equipped[slotIdx]) {
        Item* old = equipped[slotIdx];
        EffectManager::applyAll(*c, old->getEffect(), -1); // 장비 효과 제거(원복)
        items.push_back(old);
        equipped[slotIdx] = nullptr;
    }

    // 새 장비를 인벤에서 빼서 장착 슬롯에 넣고 효과 적용
    equipped[slotIdx] = newEq;
    items.erase(items.begin() + index);
    EffectManager::applyAll(*c, newEq->getEffect(), +1); // 장비 효과 적용
}

void Inventory::unequipItem(Character* c, EquipSlot slot) {
    if (!c) return; // 캐릭터 없으면 처리 불가

    const int slotIdx = (slot == EquipSlot::Weapon) ? 0 : 1;
    if (!equipped[slotIdx]) return; // 장착된 게 없으면 종료

    Item* eq = equipped[slotIdx];

    // 장비 효과 원복 후 인벤토리로 이동
    EffectManager::applyAll(*c, eq->getEffect(), -1);
    items.push_back(eq);
    equipped[slotIdx] = nullptr;
}

int Inventory::findIndexByName(const std::string& name) const {
    // 아이템 이름으로 인벤토리 내 인덱스 검색(없으면 -1)
    for (int i = 0; i < (int)items.size(); ++i) {
        if (items[i] && items[i]->getName() == name) return i;
    }
    return -1;
}

Item* Inventory::getEquipped(EquipSlot slot) const {
    // 슬롯에 장착된 아이템 포인터 반환(없으면 nullptr)
    int s = (int)slot;
    if (s < 0 || s >= (int)equipped.size()) return nullptr;
    return equipped[s];
}

bool Inventory::isEquippedName(const std::string& itemName) const {
    // 특정 이름의 아이템이 장착 중인지 확인
    for (auto* it : equipped) {
        if (it && it->getName() == itemName) return true;
    }
    return false;
}

int Inventory::size() const {
    // 인벤토리 아이템 개수 반환(장착 슬롯 제외)
    return (int)items.size();
}