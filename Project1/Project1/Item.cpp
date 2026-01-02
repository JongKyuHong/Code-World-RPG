#include "Item.h"

Item::Item(std::string name,int price, std::vector<Effect> effect) :
	name(std::move(name)), price(price), effect(std::move(effect)) {
}

std::string Item::getName() const {
	return name;
}
int Item::getPrice() const {
	return price;
}
const std::vector<Effect>& Item:: getEffect() const { return effect; }
