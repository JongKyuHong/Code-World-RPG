#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;

void SceneManager::Register(const std::string& name, Factory factory)
{
    factories[name] = std::move(factory);
}

void SceneManager::LoadScene(const std::string& name)
{
    pending = name;
}

void SceneManager::ApplyPendingIfAny()
{
    if (pending.empty()) return;

    auto it = factories.find(pending);
    if (it == factories.end()) {
        pending.clear();
        return;
    }

    current = it->second();
    if (current)
    {
        current->OnEnter();
    }
    pending.clear();
}


