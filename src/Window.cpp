#include "../inc/Window.hpp"

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
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    _window = nullptr;
}

void Window::Update() const
{
    glfwPollEvents();
}

void Window::Draw() const
{
    glfwSwapBuffers(_window);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_window);
}
