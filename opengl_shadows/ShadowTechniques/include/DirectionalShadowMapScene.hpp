#pragma once

#include "Scene.hpp"
#include "DirectionalShadowMap.hpp"

class DirectionalShadowMapScene : public Scene
{
public:
    DirectionalShadowMapScene(Window* window);
    virtual ~DirectionalShadowMapScene() override = default;

    virtual void Run() const override;
};
