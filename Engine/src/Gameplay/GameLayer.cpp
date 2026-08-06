#include <glad/glad.h>

#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Gameplay/GameLayer.h"

#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Renderer/MeshFactory.h"

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

    AssetManager::LoadMaterial(
        "Default",
        AssetManager::GetShader("Basic"));

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

        m_Scene.CreateEntity("Enemy");

        SceneSerializer::Save(
            m_Scene,
            "Assets/Scenes/Test.scene");
    }
}

void GameLayer::OnUpdate(float dt)
{
    CameraComponent* camera =
        m_Scene.FindPrimaryCamera();

    if (!camera)
        return;

    EditorLayer* editor =
        Application::Get().GetLayer<EditorLayer>();

    //------------------------------------------------
    // Ajustar aspect ratio al tamaño del Viewport
    //------------------------------------------------

    if (editor)
    {
        ImVec2 viewportSize =
            editor->GetViewportSize();

        if (viewportSize.y > 0.0f)
        {
            camera->GetCamera().SetAspectRatio(
                viewportSize.x / viewportSize.y);
        }
    }

    //------------------------------------------------
    // Permitir mover cámara solo si procede
    //------------------------------------------------

    bool allowCamera = true;

    if (editor)
    {
        if (editor->IsEditing())
            allowCamera = editor->IsViewportHovered();

        if (editor->IsPlaying())
            allowCamera = true;
    }

    if (allowCamera)
    {
        m_CameraController.Update(
            camera->GetCamera(),
            dt);
    }

    Renderer::BeginFrame();

    m_Scene.Update();

    Renderer::EndFrame();
}

void GameLayer::OnDetach()
{
    std::cout << "GameLayer Detach\n";
}

}