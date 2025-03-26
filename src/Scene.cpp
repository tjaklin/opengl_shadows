#include "../inc/Scene.hpp"

#include <cstdio>

Scene::Scene(Window* window)
    : _window(window)
{
}

void Scene::Run() const
{
    while (true)
    {
        printf("[Scene] Run called.\n");
    }
}