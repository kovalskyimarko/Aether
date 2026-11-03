#include "Aether/sceneManager.hpp"
#include <iostream>

namespace Aether
{
std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& name)
{
    auto it = scenes.find(name);
    if (it != scenes.end())
    {
        std::cerr << "WARNING::SCENE_MANAGER: Scene with name '" << name << "' already exists!\n";
        return it->second;
    }

    auto scene = std::make_shared<Scene>(name);
    scenes[name] = scene;

    if (!activeScene)
    {
        activeScene = scene;
    }

    return scene;
}

void SceneManager::LoadScene(std::shared_ptr<Scene> scene)
{
    pendingScene = scene;
}

bool SceneManager::LoadScene(const std::string& name)
{
    auto it = scenes.find(name);
    if (it != scenes.end())
    {
        pendingScene = it->second;
        return true;
    }

    std::cerr << "ERROR::SCENE_MANAGER: Cannot load scene '" << name << "', scene not found!\n";
    return false;
}

std::shared_ptr<Scene> SceneManager::GetActiveScene() const
{
    return activeScene;
}

std::shared_ptr<Scene> SceneManager::GetScene(const std::string& name) const
{
    auto it = scenes.find(name);
    if (it != scenes.end())
    {
        return it->second;
    }
    return nullptr;
}

void SceneManager::ApplyPendingScene()
{
    if (pendingScene)
    {
        activeScene = pendingScene;
        pendingScene = nullptr;
    }
}

void SceneManager::Update()
{
    ApplyPendingScene();

    if (activeScene)
    {
        activeScene->Update();
    }
}

void SceneManager::Render()
{
    if (activeScene)
    {
        activeScene->Render();
    }
}

void SceneManager::Clear()
{
    pendingScene = nullptr;
    activeScene = nullptr;
    scenes.clear();
}
}