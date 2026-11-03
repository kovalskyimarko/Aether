#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <vector>

namespace Aether {
struct Vertex {
  float posX, posY, posZ;
  float normX, normY, normZ;
  float u, v;
};

class Mesh {
public:
  Mesh(const std::vector<float> &vertices, bool hasNormals = true);

  Mesh(const std::vector<float> &vertices, const std::vector<uint32_t> &indices,
       bool hasNormals = true);

  ~Mesh();

  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  Mesh(Mesh &&other) noexcept;
  Mesh &operator=(Mesh &&other) noexcept;

  void Draw() const;

  uint32_t GetVAO() const { return VAO; }
  GLsizei GetVertexCount() const { return count; }
  bool IsIndexed() const { return EBO != 0; }

private:
  uint32_t VAO = 0;
  uint32_t VBO = 0;
  uint32_t EBO = 0;
  GLsizei count = 0;

  void SetupMesh(const std::vector<float> &vertices,
                 const std::vector<uint32_t> *indices, bool hasNormals);
};
} // namespace Aether