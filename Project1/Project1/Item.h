<<<<<<< HEAD
癤?pragma once

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

    //?꾩씠???대쫫
    virtual std::string getName() const;
    virtual int getPrice() const;

    //?꾩씠?쒖씠 ?쒖뼱???④낵瑜??대뒗吏???곗씠?곕줈 ?쒓났
    virtual const std::vector<Effect>& getEffect() const;

    //?곸젏, 罹먮┃???꾩씠???ъ씤??怨듭쑀 臾몄젣 ?닿껐 ?꾪빐 ?곸젏?????몄뒪?댁뒪 異붽?
    // ?곸젏???덈뒗  ?꾩씠?쒖쓣 ?뚮젅?댁뼱 媛쒖씤 ?뚯쑀??* ?낅┰?????꾩씠?쒖쑝濡?蹂듭궗?섍린 ?꾪빐 ?ъ슜
    virtual Item* clone() const = 0;
    bool isHealItem() const;
    bool isBuffItem() const;
    bool isEquipItem() const;
    EquipSlot getEquipSlotHint() const; // ?λ퉬硫?Weapon/Armor 諛섑솚, ?꾨땲硫?湲곕낯媛?
=======
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
>>>>>>> origin/feature/Shop
};
