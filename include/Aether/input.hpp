#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>

namespace Aether
{
class Input
{
public:
    void Init(GLFWwindow* window);
    void Update();

    bool IsKeyHold(int key) const;
    bool IsKeyPressed(int key) const;
    bool IsKeyReleased(int key) const;

    bool IsButtonHold(int button) const;
    bool IsButtonPressed(int button) const;
    bool IsButtonReleased(int button) const;

    void GetMousePosition(double& xpos, double& ypos) const;
    void GetMouseDelta(double& dx, double& dy) const;
    void GetScroll(double& xoffset, double& yoffset) const;

    void SetCursorMode(bool locked);

private:
    GLFWwindow* window = nullptr;

    std::array<bool, GLFW_KEY_LAST + 1> keys{};
    std::array<bool, GLFW_KEY_LAST + 1> keysLastFrame{};
    std::array<bool, GLFW_KEY_LAST + 1> keysJustPressed{};

    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> buttons{};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> buttonsLastFrame{};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> buttonsJustPressed{};

    double mouseX = 0.0;
    double mouseY = 0.0;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    double deltaMouseX = 0.0;
    double deltaMouseY = 0.0;

    double scrollX = 0.0;
    double scrollY = 0.0;
    bool firstMouseMove = true;

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
};
}