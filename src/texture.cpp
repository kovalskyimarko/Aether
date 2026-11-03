#include "Aether/texture.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <utility>
#include "stb_image.h"

namespace Aether
{
    Texture::Texture(const std::string& filepath)
    {
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            std::cerr << "ERROR::TEXTURE: Failed to load image at " << filepath 
                      << " (" << stbi_failure_reason() << ")" << std::endl;
            return;
        }

        GLenum internalFormat = 0;
        GLenum dataFormat = 0;

        switch (channels)
        {
            case 1:
                internalFormat = GL_R8;
                dataFormat = GL_RED;
                break;
            case 2:
                internalFormat = GL_RG8;
                dataFormat = GL_RG;
                break;
            case 3:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case 4:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            default:
                std::cerr << "ERROR::TEXTURE: Unsupported channel count (" << channels 
                          << ") in " << filepath << std::endl;
                stbi_image_free(data);
                return;
        }

        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Guarantee tight byte-packing for non-multiple-of-4 row sizes
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Restore default unpack alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    Texture::~Texture()
    {
        if (ID != 0)
        {
            if (glfwGetCurrentContext() != nullptr)
            {
                glDeleteTextures(1, &ID);
            }
            ID = 0;
        }
    }

    Texture::Texture(Texture&& other) noexcept
        : ID(other.ID), width(other.width), height(other.height), channels(other.channels)
    {
        other.ID = 0;
        other.width = 0;
        other.height = 0;
        other.channels = 0;
    }

    Texture& Texture::operator=(Texture&& other) noexcept
    {
        if (this != &other)
        {
            if (ID != 0 && glfwGetCurrentContext() != nullptr)
            {
                glDeleteTextures(1, &ID);
            }

            ID = other.ID;
            width = other.width;
            height = other.height;
            channels = other.channels;

            other.ID = 0;
            other.width = 0;
            other.height = 0;
            other.channels = 0;
        }
        return *this;
    }

    void Texture::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void Texture::Unbind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}