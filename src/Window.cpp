#include "../inc/Window.hpp"

#include <cstdio>

Window::Window(int width, int height, const char* title)
{
    if (glfwInit() != GLFW_TRUE)
    {
        // TODO: Shutdown.
        return;
    }

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_window)
    {
        // TODO: Shutdown.
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(_window);

    if (glewInit() != GLEW_OK)
    {
        // TODO: Shutdown.
        glfwTerminate();
        return;
    }

    // Enable some capabilities.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    /*
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    */
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    _window = nullptr;
}

void Window::Clear() const
{
    glClearColor(0.2f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update() const
{
    glfwPollEvents();

    // Temporary Input query mechanism.
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, GLFW_TRUE);

    else if (glfwGetKey(_window, GLFW_KEY_1) == GLFW_PRESS)
        printf("[Key] Key 1 pressed.\n");
}

void Window::Draw() const
{
    glfwSwapBuffers(_window);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_window);
}
