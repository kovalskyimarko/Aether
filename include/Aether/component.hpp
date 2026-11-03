#pragma once

#include <cstddef>
#include "Aether/math.hpp"

namespace Aether
{
    class GameObject;

    using ComponentTypeID = std::size_t;

    inline ComponentTypeID GetUniqueComponentID() noexcept
    {
        static ComponentTypeID lastID = 0;
        return lastID++;
    }

    template <typename T>
    inline ComponentTypeID GetComponentTypeID() noexcept
    {
        static_assert(!std::is_same_v<T, class Component>, "Cannot query TypeID of base Component class");
        static const ComponentTypeID typeID = GetUniqueComponentID();
        return typeID;
    }

    constexpr std::size_t MAX_COMPONENTS = 64;

    class Component
    {
    public:
        GameObject* gameObject = nullptr;
        bool enabled = true;

        virtual ~Component() = default;

        virtual void Init() {}

        virtual void Update() {}
        virtual void FixedUpdate() {}

        virtual void Render(const Mat4& /*view*/, const Mat4& /*projection*/) {}
    };
}