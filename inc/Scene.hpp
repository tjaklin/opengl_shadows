#pragma once

#include "Window.hpp"
#include "Model.hpp"
#include "VertexAttributeParser.hpp"
#include "Shader.hpp"

class Scene
{
public:
    Scene(Window* window);
    virtual ~Scene() = default;

    virtual void PreRunInitialization() const;
    virtual void Run() const;
    virtual void PostDrawHook() const;

protected:
    Window* _window;
};