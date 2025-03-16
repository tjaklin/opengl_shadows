#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    void Clear() const;
    void Update() const;
    void Draw() const;

    bool ShouldClose() const;

private:
    GLFWwindow* _window;
};