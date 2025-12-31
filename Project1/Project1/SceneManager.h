#pragma once
#include <functional>
#include<memory>
#include "Scene.h"
#include <string>
#include <unordered_map>

class SceneManager {
public:
    static SceneManager& GetInstance() {
        if (instance == nullptr) {
            instance = new SceneManager();
        }
        return *instance;
    }

    using Factory = std::function<std::unique_ptr<Scene>()>;

    void Register(const std::string& name, Factory factory);

    void LoadScene(const std::string& name);

    Scene* GetCurrent() { return current.get(); }

    void ApplyPendingIfAny();
private:
    static SceneManager* instance;
    std::unordered_map<std::string, Factory> factories;
    std::unique_ptr<Scene> current;
    std::string pending;
};