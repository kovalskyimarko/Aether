#include "Aether/shader.hpp"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Aether
{
    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (const std::exception& e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << "\n"
                      << "  Vertex Path: " << vertexPath << "\n"
                      << "  Fragment Path: " << fragmentPath << std::endl;
            return;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        if (!CheckCompileErrors(vertex, "VERTEX"))
        {
            glDeleteShader(vertex);
            return;
        }

        uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        if (!CheckCompileErrors(fragment, "FRAGMENT"))
        {
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        if (!CheckCompileErrors(ID, "PROGRAM"))
        {
            glDetachShader(ID, vertex);
            glDetachShader(ID, fragment);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteProgram(ID);
            ID = 0;
            return;
        }

        // Clean detachment and deletion
        glDetachShader(ID, vertex);
        glDetachShader(ID, fragment);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader::~Shader()
    {
        if (ID != 0)
        {
            glDeleteProgram(ID);
            ID = 0;
        }
    }

    Shader::Shader(Shader&& other) noexcept
        : ID(other.ID), uniformLocationCache(std::move(other.uniformLocationCache))
    {
        other.ID = 0;
    }

    Shader& Shader::operator=(Shader&& other) noexcept
    {
        if (this != &other)
        {
            if (ID != 0)
            {
                glDeleteProgram(ID);
            }

            ID = other.ID;
            uniformLocationCache = std::move(other.uniformLocationCache);
            other.ID = 0;
        }
        return *this;
    }

    void Shader::Use() const
    {
        if (ID != 0)
        {
            glUseProgram(ID);
        }
    }

    int Shader::GetUniformLocation(const std::string& name) const
    {
        auto it = uniformLocationCache.find(name);
        if (it != uniformLocationCache.end())
        {
            return it->second;
        }

        int location = glGetUniformLocation(ID, name.c_str());
        if (location == -1)
        {
            std::cerr << "WARNING::SHADER: Uniform '" << name << "' not found or unused.\n";
        }
        uniformLocationCache[name] = location;
        return location;
    }

    void Shader::SetMat4(const std::string& name, const Mat4& mat)
    {
        int location = GetUniformLocation(name);
        if (location != -1)
        {
            // Set GL_TRUE if your Mat4 implementation in math.hpp is row-major
            glUniformMatrix4fv(location, 1, GL_FALSE, &mat.m[0][0]);
        }
    }

    void Shader::SetInt(const std::string& name, int value)
    {
        int location = GetUniformLocation(name);
        if (location != -1)
        {
            glUniform1i(location, value);
        }
    }

    void Shader::SetFloat(const std::string& name, float value)
    {
        int location = GetUniformLocation(name);
        if (location != -1)
        {
            glUniform1f(location, value);
        }
    }

    void Shader::SetVec3(const std::string& name, const Vec3& value)
    {
        int location = GetUniformLocation(name);
        if (location != -1)
        {
            glUniform3f(location, value.x, value.y, value.z);
        }
    }

    void Shader::SetBool(const std::string& name, bool value)
    {
        int location = GetUniformLocation(name);
        if (location != -1)
        {
            glUniform1i(location, static_cast<int>(value));
        }
    }

    bool Shader::CheckCompileErrors(uint32_t shader, const std::string& type)
    {
        int success = 0;
        char infoLog[1024];

        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << std::endl;
                return false;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << std::endl;
                return false;
            }
        }
        return true;
    }
}