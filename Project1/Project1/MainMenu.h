#pragma once
#include "Scene.h"

class MainMenu : public Scene {
public:
    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(Renderer& renderer) override;

private:
    const char arrow = '>';
    int posIdx = 0;
};