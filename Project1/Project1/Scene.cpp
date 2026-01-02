#include "Scene.h"
#include "GameObject.h"

void Scene::Update(float deltaTime)
{
    for (auto& actor : actors) {
        actor->Update(deltaTime);
    }

    actors.erase(std::remove_if(actors.begin(), actors.end(),
            [](const std::unique_ptr<GameObject>& actor) {
                return actor->IsDead();
            }), actors.end());
}


void Scene::Render(Renderer& renderer)
{
    for (auto& actor : actors) {
        actor->Render(renderer);
    }
}

