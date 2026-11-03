#pragma once

#include <cstdint>
#include <string>

#include "Aether/assetsManager.hpp"
#include "Aether/input.hpp"
#include "Aether/sceneManager.hpp"
#include "Aether/time.hpp"
#include "Aether/window.hpp"

namespace Aether
{
    using uint8 = std::uint8_t;

    extern Window window;
    extern Input input;
    extern SceneManager sceneManager;
    extern Time time;
    extern AssetManager assetManager;
    extern uint32_t fallbackTextureID;

    /* Inits all main subsystems (Window, Input, Time, Assets, Fallback Textures) */
    bool Init(int w, int h, const std::string& title);

    /* Shuts down every subsystem in reverse order of initialization */
    void Shutdown();
}