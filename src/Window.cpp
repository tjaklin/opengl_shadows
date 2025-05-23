#include "Window.hpp"

#include <cstdio>

Window::Window(const char* title)
    : _camera(nullptr)
{
    if (glfwInit() != GLFW_TRUE)
    {
        return;
    }

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window dimensions are fixed for this demo program, as
    //  some parts of the code's logic depend on the windows
    //  size to be about this big.
    const int width = 1024;
    const int height = 1024;

    _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_window)
    {
        glfwTerminate();
        return;
    }

    // Create a GLFW context.
    glfwMakeContextCurrent(_window);

    // Initialize GLEW for OpenGL function pointers.
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return;
    }

    // Set fixed position.
    glfwSetWindowPos(_window, 960, 0);

    // Set OpenGL's state.
    glViewport(0, 0, width, height);

    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClearStencil(0);
    glClearColor(0.2f, 1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    _window = nullptr;
}

void Window::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::Update() const
{
    glfwPollEvents();

    // Temporary Input query mechanism.
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, GLFW_TRUE);

    // Propagate the events to _camera object.
    if (!_camera) return;
    else if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        auto movement_direction = glm::vec3(0.0f, 1.0f, 0.0f);
        _camera->HandleMovement(movement_direction);
    }
    else if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        auto movement_direction = glm::vec3(0.0f, -1.0f, 0.0f);
        _camera->HandleMovement(movement_direction);
    }
    else if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        auto movement_direction = glm::vec3(-1.0f, 0.0f, 0.0f);
        _camera->HandleMovement(movement_direction);
    }
    else if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        auto movement_direction = glm::vec3(1.0f, 0.0f, 0.0f);
        _camera->HandleMovement(movement_direction);
    }
    else if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        auto movement_direction = glm::vec3(0.0f, 0.0f, 1.0f);
        _camera->HandleMovement(movement_direction);
    }
    else if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
    {
        auto movement_direction = glm::vec3(0.0f, 0.0f, -1.0f);
        _camera->HandleMovement(movement_direction);
    }
}

void Window::Draw() const
{
    glfwSwapBuffers(_window);
}

// Set a pointer to Camera object, which will be recognised
//  as the main camera for whatever 'Scene' that's currently
//  displayed on the window screen.
// Having this pointer set enables the manipulation of
//  main camera's position in the 'Scene' space by providing
//  certain keyboard input.
//  See definition of the 'Window::Update()' method.
void Window::SetCamera(Camera* camera)
{
    if (camera)
        _camera = camera;
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_window);
}
