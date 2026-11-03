#pragma once

#include <string>
#include <cstdint>

namespace Aether
{
    class Texture
    {
    public:
        explicit Texture(const std::string& filepath);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        void Bind(uint32_t slot = 0) const;
        void Unbind(uint32_t slot = 0) const;

        bool IsLoaded() const { return ID != 0; }
        uint32_t GetID() const { return ID; }
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        int GetChannels() const { return channels; }

    private:
        uint32_t ID = 0;
        int width = 0;
        int height = 0;
        int channels = 0;
    };
}