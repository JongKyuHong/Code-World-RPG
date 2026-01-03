#pragma once
#include "Scene.h"
#include "Input.h"
#include <iostream>

class TeetActor :public GameObject {
public:
    TeetActor() :GameObject() {

    }
    void Update(float deltaTime) override {
        if(Input::GetKey(KeyCode::A)) {
            x -= 1 * deltaTime;
        }
         if (Input::GetKey(KeyCode::W)) {
             y -= 1 * deltaTime;;
        }
         if (Input::GetKey(KeyCode::S)) {
             y += 1 * deltaTime;
        }
         if(Input::GetKey(KeyCode::D)) {
            x+= deltaTime;
        }
        else {
        }
    }

    virtual void Render(Renderer& renderer) override {
        for (int i = 0; i < renderer.GetWidth(); i++) {
            for (int j = 0; j < renderer.GetHeight(); j++) {
                if(i == 0 || j == 0 || i == renderer.GetWidth() -1 || j == renderer.GetHeight() -1)
                    renderer.Put(i, j, 'o');
            }
        }
        renderer.PutBox(x, y, 5, 3);
        renderer.Put(x+1, y+1, L'��');
        renderer.Put(x, y + 1, 'T');
        renderer.Put(x + 1, y, 'T');
        renderer.PutString(x, y, "i am kang hyeonseo.");
    }

private:
    char keyChar = 'x';
    float x = 0;
    float y = 10;
    int direction = 1;
    float dTime = 0.0f;
};

class TestScene : public Scene {
public:
    virtual void OnEnter() {
        TeetActor* Teet = &Spawn<TeetActor>();
    }
};


