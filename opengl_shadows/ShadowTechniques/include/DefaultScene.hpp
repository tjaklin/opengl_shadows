#pragma once

#include "Scene.hpp"

class DefaultScene : public Scene
{
public:
    DefaultScene(Window* window);
    virtual ~DefaultScene() = default;

    virtual void Run() const override;
};
