#include "Aether/scene.hpp"
#include <algorithm>

namespace Aether
{
Scene::Scene(const std::string& name)
    : name(name), camera({0.0f, 0.0f, 0.0f})
{
}

void Scene::ProcessPendingModifications()
{
    if (!pendingRemovals.empty())
    {
        for (const auto& toRemove : pendingRemovals)
        {
            objects.erase(
                std::remove(objects.begin(), objects.end(), toRemove),
                objects.end()
            );
        }
        pendingRemovals.clear();
    }

    if (!pendingAdditions.empty())
    {
        objects.insert(objects.end(), pendingAdditions.begin(), pendingAdditions.end());
        pendingAdditions.clear();
    }
}

void Scene::Update()
{
    ProcessPendingModifications();

    for (size_t i = 0; i < objects.size(); ++i)
    {
        if (objects[i])
        {
            objects[i]->Update();
        }
    }
}

void Scene::Render()
{
    Mat4 view = camera.GetViewMatrix();
    Mat4 projection = camera.GetProjectionMatrix();

    for (size_t i = 0; i < objects.size(); ++i)
    {
        if (objects[i])
        {
            objects[i]->Render(view, projection);
        }
    }
}

std::shared_ptr<GameObject> Scene::AddObject(const std::string& name)
{
    auto obj = std::make_shared<GameObject>(name);
    pendingAdditions.push_back(obj);
    return obj;
}

void Scene::RemoveObject(const std::shared_ptr<GameObject>& obj)
{
    if (obj)
    {
        pendingRemovals.push_back(obj);
    }
}

void Scene::Clear()
{
    pendingAdditions.clear();
    pendingRemovals.clear();
    objects.clear();
}
}