#include "Engine.h"

Engine::Engine(int w, int h):renderer(w, h)
{
}

Renderer& Engine::GetRenderer()
{
    return renderer;
}
