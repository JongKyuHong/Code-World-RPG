#include "MainMenu.h"
#include "Input.h"
#include <algorithm>
#include "Engine.h"
#include <cmath>

void MainMenu::OnEnter()
{
}

void MainMenu::Update(float deltaTime)
{
    Scene::Update(deltaTime);

    if (Input::GetKeyDown(KeyCode::W))
    {
        posIdx = max(posIdx - 1, 0);
    }
    if (Input::GetKeyDown(KeyCode::S))
    {
        posIdx = min(posIdx + 1, 1);
    }
    if (Input::GetKeyDown(KeyCode::Enter)) {
        if (posIdx == 0) {
            Engine::isMainMenu = true;
            Engine::exitRequested = true;
        }
        else if (posIdx == 1) {
            Engine::isMainMenu = false;
            Engine::exitRequested = true;
        }
    }
    
    static float ttt = 0.0f;
    ttt += deltaTime;
    if (ttt > 0.1f) {
        ttt = 0.0f;
        text = (text + 1) % 6;
    }

    static float initTime = 0.0f;
    if (initTime <= 100.0f) {
        initTime += deltaTime;
    }

    if (initTime > 1.0f) {
        if (initBlankAnimationTime <= 20.0f) {
            initBlankAnimationTime += deltaTime;
        }
    }
}

void MainMenu::Render(Renderer& renderer)
{
    Scene::Render(renderer);

    renderer.PutBox(0, 0, renderer.GetWidth(), renderer.GetHeight());
    renderer.PutString(renderer.GetWidth() / 2 - 5, renderer.GetHeight() / 2, "Game Start");
    renderer.PutString(renderer.GetWidth() / 2 - 5, renderer.GetHeight() / 2 + 5, "End");
    renderer.PutString(renderer.GetWidth() / 2 - 7, renderer.GetHeight() / 2 + 5 * posIdx, ">");
    std::string s = std::to_string(text + 1) + ".txt";
    renderer.PutTextFile(5, 5, s);


    float t = (initBlankAnimationTime - 1.0f) / 0.5f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    int x1 = static_cast<int>(49 + (40 - 49) * t);
    int x2 = static_cast<int>(81 + (90 - 81) * t);

    renderer.PutTextFile(x1, 5, "TitleText.txt");
    renderer.PutTextFile(x2, 5, "TitleText2.txt");

    for (int i = 0; i < 8; i++) {
        if (initBlankAnimationTime < 1.0f - 0.05f * i) {
            renderer.PutTextFile(49, 5 + i, "BlankText.txt");
        }
    }

    renderer.PutBox(renderer.GetWidth() / 2 - 6, renderer.GetHeight() / 2 + 5 * posIdx -1 , 15, 3);
}
