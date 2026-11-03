#include "Aether/camera.hpp"
#include <algorithm>

namespace Aether
{
    Camera::Camera(Vec3 startPosition, float startYaw, float startPitch, float startFov, float startAspect)
        : position(startPosition), yaw(startYaw), pitch(startPitch),
          fov(startFov), aspect(startAspect), nearPlane(0.1f), farPlane(100.0f)
    {
        worldUp = {0.0f, 1.0f, 0.0f};
        UpdateCameraVectors();
    }

    Mat4 Camera::GetViewMatrix() const
    {
        return Mat4::LookAt(position, position + front, up);
    }

    Mat4 Camera::GetProjectionMatrix() const
    {
        return Mat4::Perspective(ToRadians(fov), aspect, nearPlane, farPlane);
    }

    void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime, float speed)
    {
        float velocity = speed * deltaTime;
        if (direction == CameraMovement::Forward)
            position += front * velocity;
        if (direction == CameraMovement::Backward)
            position -= front * velocity;
        if (direction == CameraMovement::Left)
            position -= right * velocity;
        if (direction == CameraMovement::Right)
            position += right * velocity;
        if (direction == CameraMovement::Up)
            position += worldUp * velocity;
        if (direction == CameraMovement::Down)
            position -= worldUp * velocity;
    }

    void Camera::ProcessVectorMovement(Vec3 moveDirection, float deltaTime, float speed)
    {
        Vec3 normalized = moveDirection.Normalize();
        position += normalized * (speed * deltaTime);
    }

    void Camera::ProcessMouseMovement(float xOffset, float yOffset, float sensitivity, bool constrainPitch)
    {
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (constrainPitch)
        {
            if (pitch > 89.0f)  pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        UpdateCameraVectors();
    }

    void Camera::ProcessMouseScroll(float yOffset)
    {
        fov -= yOffset;
        if (fov < 1.0f)  fov = 1.0f;
        if (fov > 90.0f) fov = 90.0f;
    }

    void Camera::UpdateCameraVectors()
    {
        Vec3 newFront;
        float yawRad = ToRadians(yaw);
        float pitchRad = ToRadians(pitch);

        newFront.x = std::cos(yawRad) * std::cos(pitchRad);
        newFront.y = std::sin(pitchRad);
        newFront.z = std::sin(yawRad) * std::cos(pitchRad);

        front = newFront.Normalize();
        right = front.Cross(worldUp).Normalize();
        up    = right.Cross(front).Normalize();
    }

    void Camera::SetAspect(float newAspect)
    {
        aspect = newAspect;
    }
}