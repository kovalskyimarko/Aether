#include "Aether/input.hpp"
#include "Aether/Aether.hpp"

namespace Aether
{
void Input::Init(GLFWwindow* win)
{
    window = win;

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);

    glfwGetCursorPos(window, &mouseX, &mouseY);
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

void Input::Update()
{
    keysLastFrame = keys;
    buttonsLastFrame = buttons;

    keysJustPressed.fill(false);
    buttonsJustPressed.fill(false);

    deltaMouseX = 0.0;
    deltaMouseY = 0.0;

    scrollX = 0.0;
    scrollY = 0.0;
}

bool Input::IsKeyHold(int key) const
{
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return keys[key];
}

bool Input::IsKeyPressed(int key) const
{
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return keysJustPressed[key] || (keys[key] && !keysLastFrame[key]);
}

bool Input::IsKeyReleased(int key) const
{
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return !keys[key] && keysLastFrame[key];
}

bool Input::IsButtonHold(int button) const
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return buttons[button];
}

bool Input::IsButtonPressed(int button) const
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return buttonsJustPressed[button] || (buttons[button] && !buttonsLastFrame[button]);
}

bool Input::IsButtonReleased(int button) const
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return !buttons[button] && buttonsLastFrame[button];
}

void Input::GetMousePosition(double& xpos, double& ypos) const
{
    xpos = mouseX;
    ypos = mouseY;
}

void Input::GetMouseDelta(double& dx, double& dy) const
{
    dx = deltaMouseX;
    dy = deltaMouseY;
}

void Input::GetScroll(double& xoffset, double& yoffset) const
{
    xoffset = scrollX;
    yoffset = scrollY;
}

void Input::SetCursorMode(bool locked)
{
    if (!window) return;
    glfwSetInputMode(window, GLFW_CURSOR, locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Input::KeyCallback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key < 0 || key > GLFW_KEY_LAST) return;

    if (action == GLFW_PRESS)
    {
        input.keys[key] = true;
        input.keysJustPressed[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        input.keys[key] = false;
    }
}

void Input::MouseButtonCallback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return;

    if (action == GLFW_PRESS)
    {
        input.buttons[button] = true;
        input.buttonsJustPressed[button] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        input.buttons[button] = false;
    }
}

void Input::ScrollCallback(GLFWwindow* /*window*/, double xoffset, double yoffset)
{
    input.scrollX += xoffset;
    input.scrollY += yoffset;
}

void Input::CursorPositionCallback(GLFWwindow* /*window*/, double xpos, double ypos)
{
    if (input.firstMouseMove)
    {
        input.lastMouseX = xpos;
        input.lastMouseY = ypos;
        input.firstMouseMove = false;
    }

    input.deltaMouseX += xpos - input.lastMouseX;
    input.deltaMouseY += ypos - input.lastMouseY;
    input.lastMouseX = xpos;
    input.lastMouseY = ypos;

    input.mouseX = xpos;
    input.mouseY = ypos;
}
}