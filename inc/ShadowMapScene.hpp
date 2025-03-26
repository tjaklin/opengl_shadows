#pragma once

#include "Scene.hpp"
#include "DirectionalShadowMap.hpp"

class ShadowMapScene : public Scene
{
public:
    ShadowMapScene(Window* window);
    virtual ~ShadowMapScene() = default;

    virtual void Run() const override;
};
