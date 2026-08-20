#include <glad/glad.h>

#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Gameplay/GameLayer.h"

#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Renderer/MeshFactory.h"
#include "AtlasEngine/Renderer/Texture.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Renderer/Model.h"

#include "AtlasEngine/Assets/AssetManager.h"

#include "AtlasEngine/Serialization/SceneSerializer.h"

#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/ModelRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"
#include "AtlasEngine/Scene/CameraComponent.h"
#include "AtlasEngine/Scene/DirectionalLightComponent.h"
#include "AtlasEngine/Scene/AmbientLightComponent.h"
#include "AtlasEngine/Scene/PointLightComponent.h"
#include "AtlasEngine/Scene/SpotLightComponent.h"

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

    AssetManager::LoadShader(
        "Shadow",
        "Assets/Shaders/Shadow.vert",
        "Assets/Shaders/Shadow.frag");

    AssetManager::LoadTexture(
        "Test",
        "Assets/Textures/Test.png");

    AssetManager::LoadTexture(
        "Test2",
        "Assets/Textures/Test2.png");

    AssetManager::LoadTexture(
        "ModelTexture",
        "Assets/Models/texture.png");

    AssetManager::LoadMaterial(
        "Default",
        AssetManager::GetShader("Basic"));

    Material* defaultMaterial =
        AssetManager::GetMaterial("Default");

    if (defaultMaterial)
    {
        defaultMaterial->SetShininess(
            32.0f);

        defaultMaterial->SetSpecularStrength(
            1.0f);
    }

    AssetManager::LoadMesh(
        "Quad",
        MeshFactory::CreateQuad());

    AssetManager::LoadMesh(
        "Sphere",
        MeshFactory::CreateSphere(
            1.0f,
            32,
            16));

    AssetManager::LoadModel(
        "CubeModel",
        "Assets/Models/cube-tex.obj");

    Model* model =
        AssetManager::GetModel(
            "CubeModel");

    if (model)
    {
        std::cout
            << "Modelo cargado desde AssetManager"
            << '\n';

        std::cout
            << "Meshes: "
            << model->GetMeshes().size()
            << '\n';
    }
    else
    {
        std::cout
            << "ERROR: no se pudo cargar CubeModel"
            << '\n';
    }

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
        // Directional Light
        //----------------------------------------------------

        Entity& directionalLight =
            m_Scene.CreateEntity(
                "DirectionalLight");

        directionalLight.AddComponent<
            DirectionalLightComponent>();

        DirectionalLightComponent& light =
            directionalLight.GetComponent<
                DirectionalLightComponent>();

        light.SetDirection({
            -0.5f,
            -1.0f,
            -0.3f
        });

        light.SetColor({
            1.0f,
            1.0f,
            1.0f
        });

        light.SetIntensity(
            1.0f);

        //----------------------------------------------------
        // Ambient Light
        //----------------------------------------------------

        Entity& ambientLight =
            m_Scene.CreateEntity(
                "AmbientLight");

        ambientLight.AddComponent<
            AmbientLightComponent>();

        AmbientLightComponent& ambient =
            ambientLight.GetComponent<
                AmbientLightComponent>();

        ambient.SetColor({
            1.0f,
            1.0f,
            1.0f
        });

        ambient.SetIntensity(
            0.15f);

        //----------------------------------------------------
        // Point Light
        //----------------------------------------------------

        Entity& pointLight =
            m_Scene.CreateEntity(
                "PointLight");

        pointLight.AddComponent<
            PointLightComponent>();

        pointLight.GetTransform().SetPosition({
            1.5f,
            1.5f,
            -1.0f
        });

        PointLightComponent& point =
            pointLight.GetComponent<
                PointLightComponent>();

        point.SetColor({
            1.0f,
            0.0f,
            0.0f
        });

        point.SetIntensity(
            0.0f);

        point.SetConstant(
            1.0f);

        point.SetLinear(
            0.09f);

        point.SetQuadratic(
            0.032f);

        //----------------------------------------------------
        // Point Light 2
        //----------------------------------------------------

        Entity& pointLight2 =
            m_Scene.CreateEntity(
                "PointLight2");

        pointLight2.AddComponent<
            PointLightComponent>();

        pointLight2.GetTransform().SetPosition({
            -1.5f,
            1.0f,
            -1.0f
        });

        PointLightComponent& point2 =
            pointLight2.GetComponent<
                PointLightComponent>();

        point2.SetColor({
            1.0f,
            1.0f,
            1.0f
        });

        point2.SetIntensity(
            0.0f);

        point2.SetConstant(
            1.0f);

        point2.SetLinear(
            0.09f);

        point2.SetQuadratic(
            0.032f);

        
        //----------------------------------------------------
        // Spot Light
        //----------------------------------------------------

        Entity& spotLight =
            m_Scene.CreateEntity(
                "SpotLight");

        spotLight.AddComponent<
            SpotLightComponent>();

        spotLight.GetTransform().SetPosition({
            0.0f,
            0.0f,
            0.0f
        });

        SpotLightComponent& spot =
            spotLight.GetComponent<
                SpotLightComponent>();

        spot.SetDirection({
            0.0f,
            0.0f,
            -1.0f
        });

        spot.SetColor({
            1.0f,
            1.0f,
            1.0f
        });

        spot.SetIntensity(
            0.0f);

        spot.SetInnerCutoff(
            12.5f);

        spot.SetOuterCutoff(
            17.5f);

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
            0.5f
        });

        player.GetTransform().SetRotation({
            30.0f,
            30.0f,
            30.0f
        });

        player.AddComponent<ModelRendererComponent>();
        player.AddComponent<MaterialComponent>();

        player.GetComponent<ModelRendererComponent>()
            .SetModel(
                AssetManager::GetModel("CubeModel"));

        player.GetComponent<MaterialComponent>()
            .SetMaterial(
                AssetManager::GetMaterial("Default"));

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

        enemy.GetTransform().SetRotation({
            0.0f,
            0.0f,
            0.0f
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

        //----------------------------------------------------
        // Sphere
        //----------------------------------------------------

        Entity& sphere =
            m_Scene.CreateEntity(
                "Sphere");

        sphere.GetTransform().SetPosition({
            -2.0f,
            0.0f,
            -3.0f
        });

        sphere.GetTransform().SetScale({
            0.75f,
            0.75f,
            0.75f
        });

        sphere.AddComponent<MeshRendererComponent>();
        sphere.AddComponent<MaterialComponent>();

        sphere.GetComponent<MeshRendererComponent>()
            .SetMesh(
                AssetManager::GetMesh("Sphere"));

        sphere.GetComponent<MaterialComponent>()
            .SetMaterial(
                AssetManager::GetMaterial("Default"));

        //----------------------------------------------------
        // Ground
        //----------------------------------------------------

        Entity& ground =
            m_Scene.CreateEntity(
                "Ground");

        ground.GetTransform().SetPosition({
            0.0f,
            -1.0f,
            -3.0f
        });

        ground.GetTransform().SetRotation({
            -90.0f,
            0.0f,
            0.0f
        });

        ground.GetTransform().SetScale({
            10.0f,
            10.0f,
            10.0f
        });

        ground.AddComponent<MeshRendererComponent>();
        ground.AddComponent<MaterialComponent>();

        ground.GetComponent<MeshRendererComponent>()
            .SetMesh(
                AssetManager::GetMesh("Quad"));

        ground.GetComponent<MaterialComponent>()
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
            editorCamera.GetProjectionMatrix(),
            editorCamera.GetPosition());
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

        for (const auto& entity :
            m_Scene.GetEntities())
        {
            if (!entity)
                continue;

            if (!entity->HasComponent<
                    SpotLightComponent>())
            {
                continue;
            }

            SpotLightComponent& spot =
                entity->GetComponent<
                    SpotLightComponent>();

            entity->GetTransform().SetPosition(
                camera->GetCamera()
                    .GetPosition());

            spot.SetDirection(
                camera->GetCamera()
                    .GetForward());

            break;
        }
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