#include "Aether/time.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace Aether
{
    void Time::Init()
    {
        lastFrameTime = glfwGetTime();
        deltaTime = 0.0f;
    }

    void Time::Update()
    {
        double currentFrameTime = glfwGetTime();
        double frameDelta = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        if (frameDelta < 0.0)
        {
            frameDelta = 0.0;
        }

        deltaTime = static_cast<float>(frameDelta);

        if (deltaTime > maxDeltaTime)
        {
            deltaTime = maxDeltaTime;
        }
    }

    float Time::GetDeltaTime() const
    {
        return deltaTime;
    }

    double Time::GetTime() const
    {
        return glfwGetTime();
    }

    float Time::GetTimeF() const
    {
        return static_cast<float>(glfwGetTime());
    }
}