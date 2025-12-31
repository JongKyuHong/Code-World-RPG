#pragma once


enum class KeyCode : int {
    a = 'a', b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
    A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
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