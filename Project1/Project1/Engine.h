#pragma once
#include "Renderer.h"
class Engine {
public:
    Engine(int w, int h);
    void Update(float deltaTime);
    void Present();

    Renderer& GetRenderer();

    bool IsRunning() const { return isRunning; };

private:
    bool isRunning = true;
    Renderer renderer;
};