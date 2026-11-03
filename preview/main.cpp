#include "Aether/Aether.hpp"
#include "Aether/meshRenderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    if (!Aether::Init(1280, 720, "Aether Engine - Model Preview"))
    {
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    bool vsyncEnabled = true;
    Aether::window.SetVSync(vsyncEnabled);
    Aether::input.SetCursorMode(true);

    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(Aether::window.GetNative(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    // Load shader, texture, and OBJ model via AssetManager
    double loadStart = glfwGetTime();
    auto shader = Aether::assetManager.LoadShader(
        "preview_shader",
        "preview/vertex.glsl",
        "preview/fragment.glsl"
    );
    auto texture = Aether::assetManager.LoadTexture(
        "model_texture",
        "preview/texture_0.png"
    );
    std::cout << "Loading mesh...\n";
    double meshStart = glfwGetTime();
    auto mesh = Aether::assetManager.LoadMesh(
        "model_mesh",
        "preview/model.obj"
    );
    std::cout << "Mesh load took: " << (glfwGetTime() - meshStart) << " s\n";
    std::cout << "Total asset load took: " << (glfwGetTime() - loadStart) << " s\n";

    if (!shader || !texture || !mesh)
    {
        std::cerr << "Failed to load one or more preview assets.\n";
        Aether::Shutdown();
        return -1;
    }

    // Create and load the preview scene
    auto scene = Aether::sceneManager.CreateScene("PreviewScene");
    Aether::sceneManager.LoadScene(scene);

    Aether::Camera& camera = scene->GetCamera();
    camera.position = {0.0f, 0.0f, 1.2f};
    camera.SetAspect(1280.0f / 720.0f);

    // Create the GameObject for the model and configure its MeshRenderer
    auto modelObj = scene->AddObject("Model");
    modelObj->GetTransform().position = {0.0f, 0.0f, 0.0f};
    modelObj->GetTransform().scale = {1.0f, 1.0f, 1.0f};

    auto* renderer = modelObj->AddComponent<Aether::MeshRenderer>();
    renderer->mesh = mesh;
    renderer->shader = shader;
    renderer->texture = texture;
    renderer->useTexture = true;
    renderer->useColor = true;
    renderer->color = {1.0f, 1.0f, 1.0f};

    int frameCount = 0;
    float fpsTimer = 0.0f;
    char titleBuffer[160];
    int skipMouseFrames = 0;

    while (!Aether::window.ShouldClose())
    {
        Aether::time.Update();
        float dt = Aether::time.GetDeltaTime();

        frameCount++;
        fpsTimer += dt;
        if (fpsTimer >= 0.25f)
        {
            float currentFps = frameCount / fpsTimer;
            float frameTimeMs = (fpsTimer / frameCount) * 1000.0f;
            std::snprintf(titleBuffer, sizeof(titleBuffer),
                          "Aether Preview - FPS: %.0f (%.2f ms) | VSync: %s [V] | Fullscreen: %s [F]",
                          currentFps, frameTimeMs, vsyncEnabled ? "ON" : "OFF",
                          Aether::window.IsFullscreen() ? "ON" : "OFF");
            glfwSetWindowTitle(Aether::window.GetNative(), titleBuffer);
            frameCount = 0;
            fpsTimer = 0.0f;
        }

        Aether::window.PollEvents();

        if (Aether::input.IsKeyPressed(GLFW_KEY_ESCAPE))
        {
            // Esc leaves fullscreen first; a second press quits
            if (Aether::window.IsFullscreen())
            {
                Aether::window.SetFullscreen(false);
                skipMouseFrames = 2;
            }
            else
            {
                break;
            }
        }

        // Toggle fullscreen with F key
        if (Aether::input.IsKeyPressed(GLFW_KEY_F))
        {
            Aether::window.ToggleFullscreen();
            // The window change can produce one big cursor jump; ignore it
            skipMouseFrames = 2;
        }

        // Keep the projection matching the current framebuffer (window resize / fullscreen)
        if (Aether::window.GetWidth() > 0 && Aether::window.GetHeight() > 0)
        {
            camera.SetAspect(static_cast<float>(Aether::window.GetWidth()) /
                             static_cast<float>(Aether::window.GetHeight()));
        }

        // Toggle VSync with V key
        if (Aether::input.IsKeyPressed(GLFW_KEY_V))
        {
            vsyncEnabled = !vsyncEnabled;
            Aether::window.SetVSync(vsyncEnabled);
        }

        // Reset camera with R key
        if (Aether::input.IsKeyPressed(GLFW_KEY_R))
        {
            camera.position = {0.0f, 0.0f, 1.2f};
            camera.yaw = -90.0f;
            camera.pitch = 0.0f;
            camera.fov = 45.0f;
            camera.ProcessMouseMovement(0.0f, 0.0f, 1.0f);
        }

        // Mouse look
        double dx = 0.0, dy = 0.0;
        Aether::input.GetMouseDelta(dx, dy);
        if (skipMouseFrames > 0)
        {
            --skipMouseFrames;
        }
        else
        {
            camera.ProcessMouseMovement(static_cast<float>(dx), static_cast<float>(-dy), 0.18f);
        }

        // Mouse scroll zoom
        double scrollX = 0.0, scrollY = 0.0;
        Aether::input.GetScroll(scrollX, scrollY);
        if (scrollY != 0.0)
        {
            camera.ProcessMouseScroll(static_cast<float>(scrollY) * 2.0f);
        }

        // WASD + Space/Ctrl/Shift camera movement
        bool isSprint = Aether::input.IsKeyHold(GLFW_KEY_LEFT_SHIFT);
        float moveSpeed = isSprint ? 6.0f : 2.5f;

        if (Aether::input.IsKeyHold(GLFW_KEY_W)) camera.ProcessKeyboard(Aether::CameraMovement::Forward, dt, moveSpeed);
        if (Aether::input.IsKeyHold(GLFW_KEY_S)) camera.ProcessKeyboard(Aether::CameraMovement::Backward, dt, moveSpeed);
        if (Aether::input.IsKeyHold(GLFW_KEY_A)) camera.ProcessKeyboard(Aether::CameraMovement::Left, dt, moveSpeed);
        if (Aether::input.IsKeyHold(GLFW_KEY_D)) camera.ProcessKeyboard(Aether::CameraMovement::Right, dt, moveSpeed);
        if (Aether::input.IsKeyHold(GLFW_KEY_SPACE)) camera.ProcessKeyboard(Aether::CameraMovement::Up, dt, moveSpeed);
        if (Aether::input.IsKeyHold(GLFW_KEY_LEFT_CONTROL) || Aether::input.IsKeyHold(GLFW_KEY_C))
            camera.ProcessKeyboard(Aether::CameraMovement::Down, dt, moveSpeed);

        Aether::sceneManager.Update();

        glClearColor(0.12f, 0.13f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Aether::sceneManager.Render();

        Aether::input.Update();
        Aether::window.SwapBuffers();
    }

    modelObj.reset();
    scene.reset();
    mesh.reset();
    texture.reset();
    shader.reset();

    Aether::Shutdown();
    return 0;
}

