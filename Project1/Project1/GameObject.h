#pragma once
#include "Renderer.h"

class GameObject {
public:
    GameObject() {

    }
    virtual ~GameObject() {}

    virtual void Update(float deltaTime) {}
    virtual void Render(Renderer& renderer) {};

    void Destroy();
    bool IsDead() const;
protected:
    bool isDead = false;
};
