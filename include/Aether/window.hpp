#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Aether
{
class Window
{
public:
    Window() = default;
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool Init(int w, int h, const std::string& title);
    void PollEvents();
    void SwapBuffers();
    bool ShouldClose() const;
    void Shutdown();

    void SetVSync(bool enabled);

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    GLFWwindow* GetNative() const { return window; }

private:
    int width = 0;
    int height = 0;
    std::string title;

    GLFWwindow* window = nullptr;
    bool initialized = false;

    static void ErrorCallback(int error, const char* description);
    static void FramebufferSizeCallback(GLFWwindow* handle, int fbWidth, int fbHeight);
};
}