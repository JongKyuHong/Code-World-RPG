#include "MainMenu.h"
#include "Input.h"
#include <algorithm>

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
}

void MainMenu::Render(Renderer& renderer)
{
    Scene::Render(renderer);

    renderer.PutBox(0, 0, renderer.GetWidth(), renderer.GetHeight());
    renderer.PutString(renderer.GetWidth() / 2 - 5, renderer.GetHeight() / 2, "Game Start");
    renderer.PutString(renderer.GetWidth() / 2 - 5, renderer.GetHeight() / 2 + 2, "End");
    renderer.PutString(renderer.GetWidth() / 2 - 7, renderer.GetHeight() / 2 + 2 * posIdx, ">");
}
