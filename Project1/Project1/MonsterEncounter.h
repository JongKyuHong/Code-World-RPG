#pragma once
#include "Scene.h"
#include "Monster.h"
#include "Character.h"

class MonsterEncounter : public Scene {
public:
    MonsterEncounter(Monster* m, Character* p);
    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(Renderer& renderer) override;

    bool IsFinished() const { return finished; }

private:
    Monster* monster;
    Character* player;
    int monsterFrame = 0;  // 애니메이션 프레임 (0~3)
    float animTime = 0.0f;  // 타이머
    float appearTime = 0.0f;  // 등장 시간
    bool finished = false;
};
