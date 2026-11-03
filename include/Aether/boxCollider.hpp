#pragma once

#include "Aether/component.hpp"
#include "Aether/math.hpp"

namespace Aether
{
    class BoxCollider : public Component
    {
    public:
        Vec3 size = {1.0f, 1.0f, 1.0f};
        Vec3 offset = {0.0f, 0.0f, 0.0f};

        BoxCollider() = default;

        Vec3 GetCenter() const;
        Vec3 GetMin() const;
        Vec3 GetMax() const;

        bool IsColliding(const BoxCollider* other) const;
        bool IsColliding(const BoxCollider& other) const;
    };
}