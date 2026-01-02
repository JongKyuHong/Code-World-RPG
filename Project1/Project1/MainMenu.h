#pragma once
#include "Scene.h"
#include "TextLoader.h"


class MainMenu : public Scene {
public:
    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(Renderer& renderer) override;

private:
    const char arrow = '>';
    int posIdx = 0;
    int runText = 0;
    int idleText = 0;

    float initBlankAnimationTime = 0.0f;
};