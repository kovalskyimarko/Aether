#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Aether/gameObject.hpp"
#include "Aether/camera.hpp"

namespace Aether
{
class Scene
{
public:
    explicit Scene(const std::string& name);
    ~Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene&&) noexcept = default;

    void Update();
    void Render();

    std::shared_ptr<GameObject> AddObject(const std::string& name);
    void RemoveObject(const std::shared_ptr<GameObject>& obj);
    void Clear();

    const std::string& GetName() const { return name; }
    Camera& GetCamera() { return camera; }
    const Camera& GetCamera() const { return camera; }

    const std::vector<std::shared_ptr<GameObject>>& GetObjects() const { return objects; }

private:
    std::string name;
    std::vector<std::shared_ptr<GameObject>> objects;
    std::vector<std::shared_ptr<GameObject>> pendingAdditions;
    std::vector<std::shared_ptr<GameObject>> pendingRemovals;

    Camera camera;

    void ProcessPendingModifications();
};
}