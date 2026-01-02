// BuffItem.cpp
#include "BuffItem.h"
#include "Character.h"
#include <iostream>
#include <utility>

BuffItem::BuffItem(std::string name, int price, std::vector<Effect> effect)
	: Item(std::move(name), price, std::move(effect))
{
}

Item* BuffItem::clone() const {
	return new BuffItem(name, price, effect);
}