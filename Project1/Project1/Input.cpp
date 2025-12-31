#include "Input.h"
#include <Windows.h>
#include <cstring>
#include <iostream>
#include <conio.h>

bool Input::pressed[256] = { false };
bool Input::down[256] = { false };

bool Input::Update()
{
    std::memcpy(down, pressed, sizeof(pressed));
    std::memset(pressed, 0, sizeof(pressed));
   
    int key;
    while(TryReadKey(key))
    {
        if (key >= 0 && key < 256)
        {
            pressed[key] = true;
        }
    }

    return true;
}

bool Input::GetKey(KeyCode key)
{
    return pressed[(int)key];
}

bool Input::GetKeyDown(KeyCode key)
{
    return down[(int)key];
}

bool Input::TryReadKey(int& outKey)
{
    if (!_kbhit())
    {
        return false;
    }
    int ch = _getch();
    if (ch == 0 || ch == 0xE0) {
        int ext = _getch();
        return true;
    }
    outKey = ch;
    return true;

    return false;
}
