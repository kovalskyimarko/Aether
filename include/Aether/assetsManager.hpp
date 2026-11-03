#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "Aether/texture.hpp"
#include "Aether/shader.hpp"
#include "Aether/mesh.hpp"

namespace Aether
{
    class AssetManager
    {
    public:
        AssetManager() = default;
        ~AssetManager();

        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;

        std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& filepath);
        std::shared_ptr<Texture> GetTexture(const std::string& name) const;
        bool HasTexture(const std::string& name) const;

        std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
        std::shared_ptr<Shader> GetShader(const std::string& name) const;
        bool HasShader(const std::string& name) const;

        std::shared_ptr<Mesh> LoadMesh(const std::string& name, const std::string& filepath);
        std::shared_ptr<Mesh> GetMesh(const std::string& name) const;
        bool HasMesh(const std::string& name) const;

        void UnloadTexture(const std::string& name);
        void UnloadShader(const std::string& name);
        void UnloadMesh(const std::string& name);

        void Clear();

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
        std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
    };
}