#include "Engine.h"
#include <chrono>
#include "SceneManager.h"
#include "TestScene.h"
#include "Input.h"
#include <Windows.h>


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

//int main() {
//    Engine engine(160, 50);
//
//    SceneManager::GetInstance().Register("TestScene", [&]() {
//        return std::make_unique<TestScene>();
//        });
//
//    using clock = std::chrono::steady_clock;
//    auto prev = clock::now();
//
//    SceneManager::GetInstance().LoadScene("TestScene");
//
//    while (engine.IsRunning())
//    {
//        auto now = clock::now();
//        std::chrono::duration<float> delta = now - prev;
//        prev = now;
//        float dt = delta.count();
//
//        engine.Update(dt);
//    }
//
//    return 0;
//}
