#include "Aether/window.hpp"
#include <iostream>

namespace Aether
{
Window::~Window()
{
    Shutdown();
}

void Window::ErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void Window::FramebufferSizeCallback(GLFWwindow* handle, int fbWidth, int fbHeight)
{
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(handle));
    if (self)
    {
        self->width = fbWidth;
        self->height = fbHeight;
    }
    glViewport(0, 0, fbWidth, fbHeight);
}

bool Window::Init(int w, int h, const std::string& t)
{
    if (initialized)
    {
        return true;
    }

    glfwSetErrorCallback(ErrorCallback);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    width = w;
    height = h;
    title = t;

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return false;
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    width = fbWidth;
    height = fbHeight;
    glViewport(0, 0, fbWidth, fbHeight);

    SetVSync(true);

    initialized = true;
    return true;
}

void Window::SetVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    if (window)
    {
        glfwSwapBuffers(window);
    }
}

bool Window::ShouldClose() const
{
    return window ? glfwWindowShouldClose(window) : true;
}

void Window::Shutdown()
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    if (initialized)
    {
        glfwTerminate();
        initialized = false;
    }
}
}