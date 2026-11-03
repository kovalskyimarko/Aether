#include "Aether/assetsManager.hpp"
#include "Aether/objLoader.hpp"
#include <iostream>

namespace Aether
{
    AssetManager::~AssetManager()
    {
        Clear();
    }

    std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& name, const std::string& filepath)
    {
        auto it = textures.find(name);
        if (it != textures.end())
        {
            return it->second;
        }

        auto texture = std::make_shared<Texture>(filepath);
        if (!texture->IsLoaded())
        {
            std::cerr << "ERROR::ASSET_MANAGER: Failed to load texture '" << name << "' from " << filepath << "\n";
            return nullptr;
        }

        textures[name] = texture;
        return texture;
    }

    std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& name) const
    {
        auto it = textures.find(name);
        if (it != textures.end())
        {
            return it->second;
        }
        std::cerr << "WARNING::ASSET_MANAGER: Texture '" << name << "' not found!\n";
        return nullptr;
    }

    bool AssetManager::HasTexture(const std::string& name) const
    {
        return textures.find(name) != textures.end();
    }

    void AssetManager::UnloadTexture(const std::string& name)
    {
        textures.erase(name);
    }

    std::shared_ptr<Shader> AssetManager::LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
    {
        auto it = shaders.find(name);
        if (it != shaders.end())
        {
            return it->second;
        }

        auto shader = std::make_shared<Shader>(vertexPath.c_str(), fragmentPath.c_str());
        if (!shader->IsValid())
        {
            std::cerr << "ERROR::ASSET_MANAGER: Failed to build shader '" << name << "'\n";
            return nullptr;
        }

        shaders[name] = shader;
        return shader;
    }

    std::shared_ptr<Shader> AssetManager::GetShader(const std::string& name) const
    {
        auto it = shaders.find(name);
        if (it != shaders.end())
        {
            return it->second;
        }
        std::cerr << "WARNING::ASSET_MANAGER: Shader '" << name << "' not found!\n";
        return nullptr;
    }

    bool AssetManager::HasShader(const std::string& name) const
    {
        return shaders.find(name) != shaders.end();
    }

    void AssetManager::UnloadShader(const std::string& name)
    {
        shaders.erase(name);
    }

    std::shared_ptr<Mesh> AssetManager::LoadMesh(const std::string& name, const std::string& filepath)
    {
        auto it = meshes.find(name);
        if (it != meshes.end())
        {
            return it->second;
        }

        std::vector<float> vertices = ObjLoader::Load(filepath);
        if (vertices.empty())
        {
            std::cerr << "ERROR::ASSET_MANAGER: Mesh '" << name << "' failed to load or has no vertices: " << filepath << "\n";
            return nullptr;
        }

        auto mesh = std::make_shared<Mesh>(vertices);
        meshes[name] = mesh;
        return mesh;
    }

    std::shared_ptr<Mesh> AssetManager::GetMesh(const std::string& name) const
    {
        auto it = meshes.find(name);
        if (it != meshes.end())
        {
            return it->second;
        }
        std::cerr << "WARNING::ASSET_MANAGER: Mesh '" << name << "' not found!\n";
        return nullptr;
    }

    bool AssetManager::HasMesh(const std::string& name) const
    {
        return meshes.find(name) != meshes.end();
    }

    void AssetManager::UnloadMesh(const std::string& name)
    {
        meshes.erase(name);
    }

    void AssetManager::Clear()
    {
        textures.clear();
        shaders.clear();
        meshes.clear();
    }
}