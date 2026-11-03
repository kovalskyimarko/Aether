#pragma once

#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <memory>
#include <iostream>
#include <cassert>
#include <type_traits>

#include "Aether/math.hpp"
#include "Aether/component.hpp"

namespace Aether
{

struct Transform
{
    Vec3 position{0.0f, 0.0f, 0.0f};
    Vec3 rotation{0.0f, 0.0f, 0.0f}; // Pitch (X), Yaw (Y), Roll (Z) in degrees
    Vec3 scale{1.0f, 1.0f, 1.0f};

    Mat4 GetModelMatrix() const
    {
        Mat4 translation = Mat4::Translate(position);
        Mat4 rotX = Mat4::RotateX(ToRadians(rotation.x));
        Mat4 rotY = Mat4::RotateY(ToRadians(rotation.y));
        Mat4 rotZ = Mat4::RotateZ(ToRadians(rotation.z));
        Mat4 scaling = Mat4::Scale(scale);

        return translation * (rotY * rotX * rotZ) * scaling;
    }
};

class GameObject
{
public:
    explicit GameObject(const std::string& name);
    ~GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    GameObject(GameObject&& other) noexcept;
    GameObject& operator=(GameObject&& other) noexcept;

    void Update();
    void Render(const Mat4& view, const Mat4& projection);

    const std::string& GetName() const { return name; }
    void SetName(const std::string& newName) { name = newName; }

    Transform& GetTransform() { return transform; }
    const Transform& GetTransform() const { return transform; }

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        ComponentTypeID typeID = GetComponentTypeID<T>();
        assert(typeID < MAX_COMPONENTS && "ComponentTypeID exceeded MAX_COMPONENTS limit!");

        if (componentBitset.test(typeID))
        {
            std::cerr << "WARNING: GameObject '" << name << "' already has component ID " << typeID << std::endl;
            return static_cast<T*>(componentsArray[typeID].get());
        }

        auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = newComponent.get();
        ptr->gameObject = this;

        componentsArray[typeID] = std::move(newComponent);
        activeComponents.push_back(ptr);
        componentBitset.set(typeID);

        ptr->Init();

        return ptr;
    }

    template<typename T>
    T* GetComponent() const
    {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        ComponentTypeID typeID = GetComponentTypeID<T>();
        assert(typeID < MAX_COMPONENTS && "ComponentTypeID exceeded MAX_COMPONENTS limit!");

        if (!componentBitset.test(typeID))
        {
            return nullptr;
        }

        return static_cast<T*>(componentsArray[typeID].get());
    }

    template<typename T>
    bool HasComponent() const
    {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        ComponentTypeID typeID = GetComponentTypeID<T>();
        return typeID < MAX_COMPONENTS && componentBitset.test(typeID);
    }

private:
    std::string name;
    Transform transform;

    std::array<std::unique_ptr<Component>, MAX_COMPONENTS> componentsArray{};
    std::bitset<MAX_COMPONENTS> componentBitset;
    std::vector<Component*> activeComponents;
};

}
