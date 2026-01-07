#include "Engine.h"
#include <chrono>
#include "SceneManager.h"
#include "TestScene.h"
#include "Input.h"
#include <Windows.h>
#include "MainMenu.h"

bool Engine::exitRequested = false;
bool Engine::isMainMenu = false;


Engine::Engine(int w, int h):renderer(w, h)
{
}
void Engine::Update(float deltaTime)
{
    renderer.Clear();

    SceneManager* sceneManager = &SceneManager::GetInstance();

    sceneManager->ApplyPendingIfAny();
    Scene* scene = sceneManager->GetCurrent();
    if (scene)
    {
        Input::Update();
        scene->Update(deltaTime);
        scene->Render(renderer);
    }

    Present();
}
void Engine::Present()
{
    renderer.Present();
}

Renderer& Engine::GetRenderer()
{
    return renderer;
}

