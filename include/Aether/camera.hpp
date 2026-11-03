#pragma once

#include "Aether/math.hpp"

namespace Aether
{
    enum class CameraMovement
    {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    class Camera
    {
    public:
        Vec3 position;
        Vec3 front;
        Vec3 up;
        Vec3 right;
        Vec3 worldUp;

        float yaw;
        float pitch;
        float fov;
        float aspect;
        float nearPlane;
        float farPlane;

        Camera(Vec3 startPosition = {0.0f, 0.0f, 0.0f},
               float startYaw = -90.0f, float startPitch = 0.0f,
               float startFov = 45.0f, float startAspect = 16.0f / 9.0f);

        Mat4 GetViewMatrix() const;
        Mat4 GetProjectionMatrix() const;

        void ProcessKeyboard(CameraMovement direction, float deltaTime, float speed);
        void ProcessVectorMovement(Vec3 moveDirection, float deltaTime, float speed);

        void ProcessMouseMovement(float xOffset, float yOffset, float sensitivity = 0.1f, bool constrainPitch = true);
        void ProcessMouseScroll(float yOffset);

        void SetAspect(float newAspect);

    private:
        void UpdateCameraVectors();
    };
}