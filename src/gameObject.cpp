#include "Aether/gameObject.hpp"

namespace Aether
{

GameObject::GameObject(const std::string& name)
    : name(name)
{
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
