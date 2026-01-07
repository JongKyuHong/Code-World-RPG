#pragma once


enum class KeyCode : int {
    A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Tab = 9,
    Enter = 13,
    ESC = 27,
    Key_0 = 48, Key_1, Key_2, Key_3, Key_4, Key_5, Key_6, Key_7, Key_8, Key_9,
    UpArrow = 72, DownArrow = 80, LeftArrow = 75, RightArrow = 77,
};

class Input {
public:
    static bool Update();
    static bool GetKey(KeyCode key);
    static bool GetKeyDown(KeyCode key);
    static bool TryReadKey(int& outKey);
private:
    static bool pressed[256];
    static bool down[256];
};

