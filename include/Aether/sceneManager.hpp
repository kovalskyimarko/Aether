#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "Aether/scene.hpp"

namespace Aether
{
class SceneManager
{
public:
    SceneManager() = default;
    ~SceneManager() = default;

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::shared_ptr<Scene> CreateScene(const std::string& name);
    
    void LoadScene(std::shared_ptr<Scene> scene);
    bool LoadScene(const std::string& name);

    std::shared_ptr<Scene> GetActiveScene() const;
    std::shared_ptr<Scene> GetScene(const std::string& name) const;

    void Update();
    void Render();

    void Clear();

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> activeScene = nullptr;
    std::shared_ptr<Scene> pendingScene = nullptr;

    void ApplyPendingScene();
};
}