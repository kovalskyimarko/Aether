#include "Aether/mesh.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Aether
{
    Mesh::Mesh(const std::vector<float>& vertices, bool hasNormals)
    {
        SetupMesh(vertices, nullptr, hasNormals);
    }

    Mesh::Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, bool hasNormals)
    {
        SetupMesh(vertices, &indices, hasNormals);
    }

    void Mesh::SetupMesh(const std::vector<float>& vertices, const std::vector<uint32_t>* indices, bool hasNormals)
    {
        if (vertices.empty())
        {
            std::cerr << "WARNING::MESH: Attempted to create mesh with 0 vertices!\n";
            return;
        }

        const size_t strideFloats = hasNormals ? 8 : 5;
        const size_t strideBytes = strideFloats * sizeof(float);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        // Upload vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        if (indices && !indices->empty())
        {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(uint32_t), indices->data(), GL_STATIC_DRAW);
            count = static_cast<GLsizei>(indices->size());
        }
        else
        {
            count = static_cast<GLsizei>(vertices.size() / strideFloats);
        }

        // Layout 0: Position (3 floats)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(strideBytes), (void*)0);
        glEnableVertexAttribArray(0);

        if (hasNormals)
        {
            // Layout 1: Texture Coords (2 floats)
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(strideBytes), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Layout 2: Normal (3 floats)
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(strideBytes), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
        }
        else
        {
            // Layout 1: Texture Coords (2 floats)
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(strideBytes), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Mesh::~Mesh()
    {
        if (glfwGetCurrentContext() != nullptr)
        {
            if (VAO != 0) glDeleteVertexArrays(1, &VAO);
            if (VBO != 0) glDeleteBuffers(1, &VBO);
            if (EBO != 0) glDeleteBuffers(1, &EBO);
        }
        VAO = 0;
        VBO = 0;
        EBO = 0;
    }

    Mesh::Mesh(Mesh&& other) noexcept
        : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), count(other.count)
    {
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.count = 0;
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept
    {
        if (this != &other)
        {
            if (glfwGetCurrentContext() != nullptr)
            {
                if (VAO != 0) glDeleteVertexArrays(1, &VAO);
                if (VBO != 0) glDeleteBuffers(1, &VBO);
                if (EBO != 0) glDeleteBuffers(1, &EBO);
            }

            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;
            count = other.count;

            other.VAO = 0;
            other.VBO = 0;
            other.EBO = 0;
            other.count = 0;
        }
        return *this;
    }

    void Mesh::Draw() const
    {
        if (VAO == 0 || count == 0) return;

        glBindVertexArray(VAO);
        if (EBO != 0)
        {
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, count);
        }
        glBindVertexArray(0);
    }
}