#pragma once

#include <memory>
#include "Aether/component.hpp"
#include "Aether/mesh.hpp"
#include "Aether/shader.hpp"
#include "Aether/texture.hpp"

namespace Aether
{
    class MeshRenderer : public Component
    {
    public:
        std::shared_ptr<Mesh> mesh = nullptr;
        std::shared_ptr<Shader> shader = nullptr;
        std::shared_ptr<Texture> texture = nullptr;

        Vec3 color = {1.0f, 1.0f, 1.0f};
        bool useColor = true;
        bool useTexture = true;

        MeshRenderer() = default;

        void Render(const Mat4& view, const Mat4& projection) override;
    };
}