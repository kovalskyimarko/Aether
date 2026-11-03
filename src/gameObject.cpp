#include "Aether/gameObject.hpp"

namespace Aether
{

GameObject::GameObject(const std::string& name)
    : name(name)
{
}

GameObject::GameObject(GameObject&& other) noexcept
    : name(std::move(other.name)),
      transform(other.transform),
      componentsArray(std::move(other.componentsArray)),
      componentBitset(other.componentBitset),
      activeComponents(std::move(other.activeComponents))
{
    for (auto* comp : activeComponents)
    {
        if (comp) comp->gameObject = this;
    }
}

GameObject& GameObject::operator=(GameObject&& other) noexcept
{
    if (this != &other)
    {
        name = std::move(other.name);
        transform = other.transform;
        componentsArray = std::move(other.componentsArray);
        componentBitset = other.componentBitset;
        activeComponents = std::move(other.activeComponents);
        for (auto* comp : activeComponents)
        {
            if (comp) comp->gameObject = this;
        }
    }
    return *this;
}

void GameObject::Update()
{
    for (auto* comp : activeComponents)
    {
        if (comp && comp->enabled)
        {
            comp->Update();
        }
    }
}

void GameObject::Render(const Mat4& view, const Mat4& projection)
{
    for (auto* comp : activeComponents)
    {
        if (comp && comp->enabled)
        {
            comp->Render(view, projection);
        }
    }
}

}
