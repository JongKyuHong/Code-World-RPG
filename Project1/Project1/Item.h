#pragma once
#include <string>

class Character;

enum class ItemType
{
    HealPotion,
    AtkPotion
};

class Item
{
protected:
    std::string name;
    ItemType type;
    int price; 

public:
    Item(const std::string& name, ItemType type, int price)
        : name(name), type(type), price(price) {
    }

    virtual ~Item() = default; 

    std::string getName() const { return name; }

    int getPrice() const { return price; }

    ItemType getType() const { return type; }

    virtual void use(Character* character) = 0;
};


class HealthPotion : public Item
{
    int healAmount;

public:
    HealthPotion(const std::string& name, int price, int healAmount) // 체력포션
        : Item(name, ItemType::HealPotion, price), healAmount(healAmount) {
    }

    void use(Character* character) override
    {
        (void)character;
    }
};

class AttackPotion : public Item
{
    int atkAmount;

public: 
    AttackPotion(const std::string& name, int price, int atkAmount) // 공격물약
        : Item(name, ItemType::AtkPotion, price), atkAmount(atkAmount) {
    }

    void use(Character* character) override
    {
        (void)character;
    }
};

class LongSword : public Item
{
    int healAmount;

public:
     LongSword(const std::string& name, int price, int healAmount) // 체력포션
        : Item(name, ItemType::HealPotion, price), healAmount(healAmount) {
    }

    void use(Character* character) override
    {
        (void)character;
    }
}; 

class HealthPotion : public Item
{
    int healAmount;

public:
    HealthPotion(const std::string& name, int price, int healAmount) // 체력포션
        : Item(name, ItemType::HealPotion, price), healAmount(healAmount) {
    }

    void use(Character* character) override
    {
        (void)character;
    }
};

