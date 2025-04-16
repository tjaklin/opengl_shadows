#pragma once

#include "Scene.hpp"
#include "VarianceShadowMap.hpp"

class VarianceShadowMapScene : public Scene
{
public:
    VarianceShadowMapScene(Window* window);
    ~VarianceShadowMapScene() override = default;

    void Run() const override;
};