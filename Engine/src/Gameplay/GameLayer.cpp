#include <glad/glad.h>

#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Gameplay/GameLayer.h"

#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Renderer/MeshFactory.h"
#include "AtlasEngine/Renderer/Texture.h"
#include "AtlasEngine/Renderer/Material.h"

#include "AtlasEngine/Assets/AssetManager.h"

#include "AtlasEngine/Serialization/SceneSerializer.h"

#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"
#include "AtlasEngine/Scene/CameraComponent.h"

#include "AtlasEngine/Editor/EditorLayer.h"

#include "AtlasEngine/Systems/RenderSystem.h"

#include <iostream>

namespace AtlasEngine
{

void GameLayer::OnAttach()
{
    std::cout << "GameLayer Attach\n";

    m_Scene.AddSystem<RenderSystem>();

    //----------------------------------------------------
    // Recursos
    //----------------------------------------------------

    AssetManager::LoadShader(
        "Basic",
        "Assets/Shaders/Basic.vert",
        "Assets/Shaders/Basic.frag");

    AssetManager::LoadTexture(
        "Test",
        "Assets/Textures/Test.png");

    AssetManager::LoadTexture(
        "Test2",
        "Assets/Textures/Test2.png");

    AssetManager::LoadMaterial(
        "Default",
        AssetManager::GetShader("Basic"));

    Material* defaultMaterial =
        AssetManager::GetMaterial("Default");

    if (defaultMaterial)
    {
        defaultMaterial->SetTexture(
            AssetManager::GetTexture("Test2"));
    }

    AssetManager::LoadMesh(
        "Quad",
        MeshFactory::CreateQuad());

    if (SceneSerializer::Load(
        m_Scene,
        "Assets/Scenes/Test.scene"))
    {
        std::cout << "Escena cargada\n";
    }
    else
    {
        //----------------------------------------------------
        // Camera Entity
        //----------------------------------------------------

        Entity& camera =
            m_Scene.CreateEntity("MainCamera");

        camera.AddComponent<CameraComponent>();

        CameraComponent& cameraComponent =
            camera.GetComponent<CameraComponent>();

        cameraComponent.SetPrimary(true);

        Window& window =
            Application::Get().GetWindow();

        cameraComponent.GetCamera().SetAspectRatio(
            window.GetWidth() /
            static_cast<float>(window.GetHeight()));

        cameraComponent.GetCamera().SetPosition({
            0.0f,
            0.0f,
            0.0f
        });

        cameraComponent.GetCamera().SetRotation({
            0.0f,
            0.0f,
            0.0f
        });

        //----------------------------------------------------
        // Player
        //----------------------------------------------------

        Entity& player =
            m_Scene.CreateEntity("Player");

        player.GetTransform().SetPosition({
            0.0f,
            0.0f,
            -2.0f
        });

        player.GetTransform().SetScale({
            0.5f,
            0.5f,
            1.0f
        });

        player.GetTransform().SetRotation({
            30.0f,
            30.0f,
            30.0f
        });

        player.AddComponent<MeshRendererComponent>();
        player.AddComponent<MaterialComponent>();

        player.GetComponent<MeshRendererComponent>()
            .SetMesh(AssetManager::GetMesh("Quad"));

        player.GetComponent<MaterialComponent>()
            .SetMaterial(AssetManager::GetMaterial("Default"));

        //----------------------------------------------------
        // Enemy
        //----------------------------------------------------

        Entity& enemy =
            m_Scene.CreateEntity("Enemy");

        enemy.GetTransform().SetPosition({
            2.0f,
            0.0f,
            -2.0f
        });

        enemy.GetTransform().SetScale({
            0.5f,
            0.5f,
            1.0f
        });

        enemy.AddComponent<MeshRendererComponent>();
        enemy.AddComponent<MaterialComponent>();

        enemy.GetComponent<MeshRendererComponent>()
            .SetMesh(
                AssetManager::GetMesh("Quad"));

        enemy.GetComponent<MaterialComponent>()
            .SetMaterial(
                AssetManager::GetMaterial("Default"));
    }
}

void GameLayer::OnUpdate(float dt)
{
    EditorLayer* editor =
        Application::Get()
            .GetLayer<EditorLayer>();


    CameraComponent* camera =
        m_Scene.FindPrimaryCamera();


    RenderSystem& renderSystem =
        m_Scene.GetSystem<RenderSystem>();


    // ====================================================
    // EDIT MODE
    // ====================================================

    if (editor &&
        editor->IsEditing())
    {
        const EditorCamera& editorCamera =
            editor->GetEditorCamera();


        renderSystem.SetViewProjection(
            editorCamera.GetViewMatrix(),
            editorCamera.GetProjectionMatrix());
    }
    else
    {
        // =================================================
        // PLAY MODE
        // =================================================

        if (!camera)
            return;


        renderSystem.ClearViewProjection();


        if (editor)
        {
            ImVec2 viewportSize =
                editor->GetViewportSize();


            if (viewportSize.x > 0.0f &&
                viewportSize.y > 0.0f)
            {
                camera->GetCamera()
                    .SetAspectRatio(
                        viewportSize.x /
                        viewportSize.y);


                Renderer::ResizeFramebuffer(
                    static_cast<uint32_t>(
                        viewportSize.x),

                    static_cast<uint32_t>(
                        viewportSize.y));
            }
        }


        m_CameraController.Update(
            camera->GetCamera(),
            dt);
    }


    // ====================================================
    // RENDER
    // ====================================================

    Renderer::BeginFrame();


    m_Scene.Update();


    Renderer::EndFrame();
}

void GameLayer::OnDetach()
{
    std::cout << "GameLayer Detach\n";
}

}