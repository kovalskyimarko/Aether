#include "Aether/meshRenderer.hpp"
#include "Aether/gameObject.hpp"
#include "Aether/Aether.hpp"
#include <glad/glad.h>

namespace Aether
{
    void MeshRenderer::Render(const Mat4& view, const Mat4& projection)
    {
        if (!mesh || !shader || !gameObject) return;

        const Transform& t = gameObject->GetTransform();
        Mat4 model = t.GetModelMatrix();

        shader->Use();
        shader->SetMat4("model", model);
        shader->SetMat4("view", view);
        shader->SetMat4("projection", projection);
        shader->SetVec3("objectColor", color);
        shader->SetBool("useColor", useColor);

        // Texture binding logic
        if (texture && useTexture)
        {
            texture->Bind(0);
            shader->SetInt("texture1", 0);
            shader->SetBool("useTexture", true);
        }
        else
        {
            // Keep the 1x1 white fallback texture bound on unit 0 so the driver
            // never sees an unloadable/unbound texture on active sampler uniforms
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Aether::fallbackTextureID);
            shader->SetInt("texture1", 0);
            shader->SetBool("useTexture", useTexture);
        }

        mesh->Draw();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Aether::fallbackTextureID);
    }
}