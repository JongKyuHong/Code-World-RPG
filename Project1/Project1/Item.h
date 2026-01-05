#pragma once

#include <string>

class Character;

class Item
{
public:
    // 아이템 이름
    virtual std::string getName() = 0;
    
    // 아이템 사용
    virtual void use(Character* character) = 0;

    virtual ~Item() {}
};


