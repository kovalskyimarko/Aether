#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include "Aether/math.hpp"

namespace Aether
{
    class Shader
    {
    public:
        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();

        // Non-copyable
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        // Moveable
        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        void Use() const;

        void SetMat4(const std::string& name, const Mat4& mat);
        void SetInt(const std::string& name, int value);
        void SetFloat(const std::string& name, float value);
        void SetVec3(const std::string& name, const Vec3& value);
        void SetBool(const std::string& name, bool value);

        uint32_t GetID() const { return ID; }
        bool IsValid() const { return ID != 0; }

    private:
        uint32_t ID = 0;
        mutable std::unordered_map<std::string, int> uniformLocationCache;

        int GetUniformLocation(const std::string& name) const;
        bool CheckCompileErrors(uint32_t shader, const std::string& type);
    };
}