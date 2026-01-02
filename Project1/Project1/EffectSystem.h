#pragma once
#include <vector>
#include "Types.h"
#include "Character.h"
#include "EffectManager.h"

class EffectSystem
{
public:
	struct Active {
		Effect effect;
		int remaining;
	};
private:
	std::vector<Active> active;
public:
	void applyFromItem(Character& c, const std::vector<Effect>& effect, EffectManager& manager);

	void updateActiveEffects(Character& c, EffectManager& manager);

	int activeCount() const { return(int)active.size(); }

	void clearAll(Character& c, EffectManager& manager);
};


