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
        if (key >= 'a' && key <= 'z')
        {
            key = key - 'a' + 'A';
        }

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
    return !down[(int)key] && pressed[(int)key];
}

bool Input::TryReadKey(int& outKey)
{
    for (int i = 0; i < 256; i++) {
        pressed[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }
    return false;


    if (!_kbhit())
    {
        return false;
    }
    int ch = _getch();
    if (ch == 0 || ch == 0xE0) {
        int ext = _getch();
        switch (ext)
        {
            case 72:
                outKey = (int)KeyCode::UpArrow;
                return true;
            case 80:
                outKey = (int)KeyCode::DownArrow;
                return true;
            case 75:
                outKey = (int)KeyCode::LeftArrow;
                return true;
            case 77:
                outKey = (int)KeyCode::RightArrow;
                return true;

        }
        return true;
    }
    outKey = ch;
    return true;

    return false;
}

