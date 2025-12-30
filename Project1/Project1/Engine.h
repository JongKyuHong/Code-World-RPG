#pragma once
#include "Renderer.h"
class Engine {
public:
    Engine(int w, int h);
    void Update(float deltaTime);
    void Present();

    Renderer& GetRenderer();

private:
    bool isRunning = true;
    Renderer renderer;
};