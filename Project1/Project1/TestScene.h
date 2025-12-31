#pragma once
#include "Scene.h"

class TeetActor :public GameObject {
public:
    TeetActor() :GameObject() {

    }
    void Update(float deltaTime) override {
        dTime += deltaTime;
        if (dTime > 0.05f) {
            dTime = 0.0f;
            x += direction;
            if (x == 160 || x == 0) {
                direction *= -1;
            }
        }
    }

    virtual void Render(Renderer& renderer) override {
        for (int i = 0; i < renderer.GetWidth(); i++) {
            for (int j = 0; j < renderer.GetHeight(); j++) {
                if(i == 0 || j == 0 || i == renderer.GetWidth() -1 || j == renderer.GetHeight() -1)
                    renderer.Put(i, j, 'o');
            }
        }


        renderer.Put(x, y, 'T');
        renderer.Put(x+1, y+1, 'T');
        renderer.Put(x, y + 1, 'T');
        renderer.Put(x + 1, y, 'T');
    }

private:
    int x = 0;
    int y = 10;
    int direction = 1;
    float dTime = 0.0f;
};

class TestScene : public Scene {
public:
    virtual void OnEnter() {
        TeetActor* Teet = &Spawn<TeetActor>();

    }
};

