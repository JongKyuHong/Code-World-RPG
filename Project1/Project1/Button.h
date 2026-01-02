#pragma once
#include "GameObject.h"

class Button :public GameObject {
public:
    Button(std::string IntButtonText) :GameObject(), buttonText(IntButtonText){

    }
    void Update(float deltaTime) override;

    virtual void Render(Renderer& renderer) override;

    void Select();
    void UnSelect();

private:
    std::string buttonText;
    bool isSelected = false;
};