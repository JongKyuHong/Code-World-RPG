// EffectApplier.h
#pragma once
#include "Types.h"
#include "Character.h" 
class EffectManager {
public:
    static void apply(Character& c, const Effect& e, int sign);
    static void applyAll(Character& c,
        const std::vector<Effect>& effect,
        int sign);
};