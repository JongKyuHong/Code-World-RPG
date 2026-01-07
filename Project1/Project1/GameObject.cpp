#include "GameObject.h"

void GameObject::Destroy()
{
    isDead = true;
}

bool GameObject::IsDead() const
{
    return isDead;
}

