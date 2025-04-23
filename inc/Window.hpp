#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"

class Window
{
public:
    Window(const char* title);
    ~Window();

    void Clear() const;
    void Update() const;
    void Draw() const;

    void SetCamera(Camera* camera);
    bool ShouldClose() const;

private:
    GLFWwindow* _window;
    Camera* _camera;
};