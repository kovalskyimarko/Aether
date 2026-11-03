#include "Aether/Aether.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Aether {
Window window;
Input input;
SceneManager sceneManager;
Time time;
AssetManager assetManager;
uint32_t fallbackTextureID = 0;

bool Init(int w, int h, const std::string &title) {
  if (!window.Init(w, h, title)) {
    return false;
  }

  input.Init(window.GetNative());
  time.Init();

  // 1x1 solid white fallback texture
  glGenTextures(1, &fallbackTextureID);
  glBindTexture(GL_TEXTURE_2D, fallbackTextureID);

  const unsigned char whitePixel[4] = {255, 255, 255, 255};

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               whitePixel);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  return true;
}

void Shutdown() {
  sceneManager.Clear();
  assetManager.Clear();

  if (fallbackTextureID != 0) {
    glDeleteTextures(1, &fallbackTextureID);
    fallbackTextureID = 0;
  }

  window.Shutdown();
}
} // namespace Aether