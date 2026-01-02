#pragma once
#include "Item.h"
#include "Types.h"
#include "utility"
class BuffItem : public Item
{
public:
	BuffItem(std::string name, int price, std::vector<Effect> effect);
	Item* clone() const override;
};