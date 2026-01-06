// HealItem.h
#pragma once
#include "Item.h"
#include "Types.h"
#include <vector>
#include <utility>

class HealItem : public Item {
public:
    //일반 생성자: healAmount로 1개 effect 생성
    HealItem(std::string name, int price, int healAmount);

    //내부용 생성자: effects로 직접 주입 (clone에 사용)
    HealItem(std::string name, int price, std::vector<Effect> effect);

    //상점용 동일 아이템 새 인스턴스 생성
    Item* clone() const override;
};

