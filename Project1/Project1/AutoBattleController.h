#pragma once
#include <vector>          // std::vector 사용
#include "Types.h"         // Effect, EffectType, StatType

class Character;
class Monster;
class Inventory;
class Item;

enum class PlayerActionType {
    Attack,
    UseItem
};

struct PlayerAction {
    PlayerActionType type = PlayerActionType::Attack;
    int inventoryIndex = -1;
};

struct AutoBattleConfig {
    double healHpRatio = 0.60;
    double emergencyHpRatio = 0.18;
    int maxItemsPerTurn = 1;
    int maxItemsPerBattle = 3;

    bool allowInvincibleOnBoss = true;
    bool allowStatBuffAutoUse = false;
};

class AutoBattleController final {
public:
    explicit AutoBattleController(AutoBattleConfig cfg = {});

    void onBattleStart(bool isBoss);
    void onTurnStart(int turnCount);

    PlayerAction decide(Character& player, Monster& monster, Inventory& inv, bool isBoss);

    void onItemUsed();

private:
    AutoBattleConfig cfg_;
    int usedItemsThisBattle_ = 0;
    int usedItemsThisTurn_ = 0;
    int turnCount_ = 0;

private:
    bool canUseItemNow() const;
    bool hpBelow(Character& player, double ratio) const;

    int findBestHealItemIndex(Inventory& inv) const;
    int findBestBuffItemIndex(Inventory& inv) const;

    bool isCandidateConsumable(const Item* it) const;
    const std::vector<Effect>& getEffects(const Item* it) const;

    bool isAutoUsableAllowed(const Item* it) const;
};
