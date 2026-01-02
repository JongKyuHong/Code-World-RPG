#pragma once

#include "Character.h"
#include "Types.h"
#include<vector>
#include<string>
class Item
{
protected:
    std::string name;
    int price;
    std::vector<Effect> effect;
public:
    Item(std::string name, int price, std::vector<Effect> effect);

    virtual ~Item() = default;

    //아이템 이름
    virtual std::string getName() const;
    virtual int getPrice() const;

    //아이템이 “어떤 효과를 내는지” 데이터로 제공
    virtual const std::vector<Effect>& getEffect() const;

    //상점, 캐릭터 아이템 포인터 공유 문제 해결 위해 상점용 새 인스턴스 추가
    // 상점에 있는  아이템을 플레이어 개인 소유의** 독립된 새 아이템으로 복사하기 위해 사용
    virtual Item* clone() const = 0;
};
