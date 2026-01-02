#pragma once
#include "Renderer.h"


class Engine {
public:
    Engine(int w, int h);
    void Update(float deltaTime);
    void Present();

    Renderer& GetRenderer();

    bool IsRunning() const { return isRunning; };

    static bool exitRequested;
    static bool isMainMenu;
private:
    bool isRunning = true;
    Renderer renderer;
};