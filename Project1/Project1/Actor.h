#pragma once

class GameObject {
public:
    virtual ~GameObject() {}

    virtual void Update(float deltaTime) {}
    virtual void Render() {};

    void Destroy();
    bool IsDead() const;
protected:
    bool isDead = false;
};