#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"

class Renderer;

class Scene {
protected:
    bool isRunning = true;
public:
    virtual ~Scene() {}
    virtual void OnEnter() {}
    virtual void Update(float deltaTime);
    virtual void Render(Renderer& renderer);
    
    template<typename T, typename... Args>
    T& Spawn(Args&&... args) {
        static_assert(std::is_base_of<GameObject, T>::value, "Not GameObject");
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *obj;
        actors.push_back(std::move(obj));
        return ref;
    }
protected:
    std::vector<std::unique_ptr<GameObject>> actors;
};

