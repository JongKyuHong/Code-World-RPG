#pragma once
#include "Scene.h"
#include "Monster.h"
#include "Character.h"
#include "BattleResult.h"

class NormalBattleScene : public Scene {
public:
    NormalBattleScene(Monster* m, Character* p);
    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(Renderer& renderer) override;
    bool IsFinished() const { return battleEnded; }
    const BattleResult& GetResult() const { return result; }

private:
    enum BattleState {
        PLAYER_SELECT,
        PLAYER_ATTACK,
        MONSTER_ATTACK,
        INVENTORY_VIEW,
        SHOP_VIEW,
        RESULT
    };

    Monster* monster;
    Character* player;
    BattleState state = PLAYER_SELECT;
    bool battleEnded = false;
    BattleResult result;
    int selectedAction = 0;
};
