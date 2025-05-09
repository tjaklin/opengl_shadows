#pragma once

#include "Scene.hpp"
#include "OmnidirectionalShadowMap.hpp"

class OmnidirectionalShadowMapScene : public Scene
{
public:
    OmnidirectionalShadowMapScene(Window* window);
    ~OmnidirectionalShadowMapScene() override = default;

    void Run() const override;
};