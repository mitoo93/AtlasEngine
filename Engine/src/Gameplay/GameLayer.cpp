#include <glad/glad.h>

#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Gameplay/GameLayer.h"

#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Renderer/MeshFactory.h"
#include "AtlasEngine/Renderer/Texture.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Renderer/Model.h"
#include "AtlasEngine/Renderer/Cubemap.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Mesh.h"

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
#include <string>

namespace AtlasEngine
{

void GameLayer::OnAttach()
{
    std::cout << "GameLayer Attach\n";

    m_Scene.AddSystem<RenderSystem>();

    //----------------------------------------------------
    // Shaders
    //----------------------------------------------------

    AssetManager::LoadShader(
        "Basic",
        "Assets/Shaders/Basic.vert",
        "Assets/Shaders/Basic.frag");

    AssetManager::LoadShader(
        "PBR",
        "Assets/Shaders/PBR.vert",
        "Assets/Shaders/PBR.frag");

    AssetManager::LoadShader(
        "Shadow",
        "Assets/Shaders/Shadow.vert",
        "Assets/Shaders/Shadow.frag");

    AssetManager::LoadShader(
        "Skybox",
        "Assets/Shaders/Skybox.vert",
        "Assets/Shaders/Skybox.frag");

    AssetManager::LoadShader(
        "ToneMapping",
        "Assets/Shaders/ToneMapping.vert",
        "Assets/Shaders/ToneMapping.frag");

    AssetManager::LoadShader(
        "BrightPass",
        "Assets/Shaders/BrightPass.vert",
        "Assets/Shaders/BrightPass.frag");

    AssetManager::LoadShader(
        "GaussianBlur",
        "Assets/Shaders/GaussianBlur.vert",
        "Assets/Shaders/GaussianBlur.frag");

    AssetManager::LoadShader(
        "BloomComposite",
        "Assets/Shaders/BloomComposite.vert",
        "Assets/Shaders/BloomComposite.frag");

    AssetManager::LoadShader(
        "Irradiance",
        "Assets/Shaders/Irradiance.vert",
        "Assets/Shaders/Irradiance.frag");

    AssetManager::LoadShader(
        "Prefilter",
        "Assets/Shaders/Prefilter.vert",
        "Assets/Shaders/Prefilter.frag");

    AssetManager::LoadShader(
        "BRDF",
        "Assets/Shaders/BRDF.vert",
        "Assets/Shaders/BRDF.frag");

    //----------------------------------------------------
    // Textures
    //----------------------------------------------------

    AssetManager::LoadTexture(
        "Test",
        "Assets/Textures/Test.png");

    AssetManager::LoadTexture(
        "Test2",
        "Assets/Textures/Test2.png");

    AssetManager::LoadTexture(
        "ModelTexture",
        "Assets/Models/texture.png");

    AssetManager::LoadTexture(
        "MetalPlate_Albedo",
        "Assets/Textures/PBR/MetalPlate/albedo.png");

    AssetManager::LoadTexture(
        "MetalPlate_Normal",
        "Assets/Textures/PBR/MetalPlate/normal.png");

    AssetManager::LoadTexture(
        "MetalPlate_Metallic",
        "Assets/Textures/PBR/MetalPlate/metallic.png");

    AssetManager::LoadTexture(
        "MetalPlate_Roughness",
        "Assets/Textures/PBR/MetalPlate/roughness.png");

    AssetManager::LoadTexture(
        "MetalPlate_AO",
        "Assets/Textures/PBR/MetalPlate/ao.png");


    //----------------------------------------------------
    // Basic Material
    //----------------------------------------------------

    AssetManager::LoadMaterial(
        "Default",
        AssetManager::GetShader("Basic"));

    Material* defaultMaterial =
        AssetManager::GetMaterial(
            "Default");

    if (defaultMaterial)
    {
        defaultMaterial->SetShininess(
            32.0f);

        defaultMaterial->SetSpecularStrength(
            1.0f);
    }


    //----------------------------------------------------
    // PBR Material
    //----------------------------------------------------

    AssetManager::LoadMaterial(
        "PBR_Default",
        AssetManager::GetShader("PBR"));

    Material* pbrMaterial =
        AssetManager::GetMaterial(
            "PBR_Default");

    if (pbrMaterial)
    {
        pbrMaterial->SetBaseColor({
            1.0f,
            1.0f,
            1.0f
        });

        pbrMaterial->SetMetallic(
            0.0f);

        pbrMaterial->SetRoughness(
            0.5f);

        pbrMaterial->SetAmbientOcclusion(
            1.0f);


        pbrMaterial->SetTexture(
            AssetManager::GetTexture(
                "MetalPlate_Albedo"));

        pbrMaterial->SetNormalTexture(
            AssetManager::GetTexture(
                "MetalPlate_Normal"));

        pbrMaterial->SetMetallicTexture(
            AssetManager::GetTexture(
                "MetalPlate_Metallic"));

        pbrMaterial->SetRoughnessTexture(
            AssetManager::GetTexture(
                "MetalPlate_Roughness"));

        pbrMaterial->SetAOTexture(
            AssetManager::GetTexture(
                "MetalPlate_AO"));
    }

    //----------------------------------------------------
    // Cube Map
    //----------------------------------------------------

    AssetManager::LoadCubemap(
        "DefaultSkybox",
        {
            "Assets/Skyboxes/Default/right.jpg",
            "Assets/Skyboxes/Default/left.jpg",
            "Assets/Skyboxes/Default/top.jpg",
            "Assets/Skyboxes/Default/bottom.jpg",
            "Assets/Skyboxes/Default/front.jpg",
            "Assets/Skyboxes/Default/back.jpg"
        });


    //----------------------------------------------------
    // Meshes
    //----------------------------------------------------

    AssetManager::LoadMesh(
        "Quad",
        MeshFactory::CreateQuad());

    AssetManager::LoadMesh(
        "Sphere",
        MeshFactory::CreateSphere(
            1.0f,
            32,
            16));

    AssetManager::LoadMesh(
        "SkyboxCube",
        MeshFactory::CreateSkyboxCube());

    if (!Renderer::InitBRDFLUT())
    {
        std::cout
            << "ERROR inicializando BRDF LUT\n";
    }


    //----------------------------------------------------
    // IBL - Irradiance Map
    //----------------------------------------------------

    AssetManager::CreateCubemap(
        "DefaultIrradiance",
        32,
        false);

    Cubemap* environmentMap =
        AssetManager::GetCubemap(
            "DefaultSkybox");

    Cubemap* irradianceMap =
        AssetManager::GetCubemap(
            "DefaultIrradiance");

    Shader* irradianceShader =
        AssetManager::GetShader(
            "Irradiance");

    Mesh* skyboxCube =
        AssetManager::GetMesh(
            "SkyboxCube");

    if (environmentMap &&
        irradianceMap &&
        irradianceShader &&
        skyboxCube)
    {
        if (irradianceMap->GenerateIrradianceMap(
                *environmentMap,
                *irradianceShader,
                *skyboxCube,
                32))
        {
            std::cout
                << "Irradiance map generado correctamente\n";
        }
        else
        {
            std::cout
                << "ERROR generando irradiance map\n";
        }
    }
    else
    {
        std::cout
            << "ERROR: faltan recursos para generar Irradiance Map\n";
    }

    //----------------------------------------------------
    // IBL - PREFILTER MAP
    //----------------------------------------------------

    AssetManager::CreateCubemap(
        "DefaultPrefilter",
        128,
        true);

    Cubemap* prefilterMap =
        AssetManager::GetCubemap(
            "DefaultPrefilter");

    Shader* prefilterShader =
        AssetManager::GetShader(
            "Prefilter");

    if (environmentMap &&
        prefilterMap &&
        prefilterShader &&
        skyboxCube)
    {
        if (prefilterMap->GeneratePrefilterMap(
                *environmentMap,
                *prefilterShader,
                *skyboxCube,
                128,
                5))
        {
            std::cout
                << "Prefilter map generado correctamente\n";
        }
        else
        {
            std::cout
                << "ERROR generando prefilter map\n";
        }
    }
    else
    {
        std::cout
            << "ERROR: faltan recursos para generar Prefilter Map\n";
    }


    //----------------------------------------------------
    // Models
    //----------------------------------------------------

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
            2.0f);

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
            0.05f);

        //----------------------------------------------------
        // Point Light
        //----------------------------------------------------

        Entity& pointLight =
            m_Scene.CreateEntity(
                "PointLight");

        pointLight.AddComponent<
            PointLightComponent>();

        pointLight.GetTransform().SetPosition({
            2.0f,
            3.0f,
            2.0f
        });

        PointLightComponent& point =
            pointLight.GetComponent<
                PointLightComponent>();

        point.SetColor({
            1.0f,
            1.0f,
            1.0f
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
                AssetManager::GetMaterial(
                    "PBR_Default"));

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

    Renderer::SetExposure(
        1.0f);

        // ----------------------------------------------------
        // PRUEBA DE RENDER QUEUE
        // ----------------------------------------------------

        constexpr bool enableRenderQueueTest = false;

        if (enableRenderQueueTest)
        {
            Mesh* sharedMesh =
                AssetManager::GetMesh("Sphere");

            Material* materialA =
                AssetManager::GetMaterial("Default");

            Material* materialB =
                AssetManager::GetMaterial("PBR_Default");

            if (sharedMesh &&
                materialA &&
                materialB &&
                materialA->GetShader() &&
                materialB->GetShader())
            {
                constexpr int columns = 10;
                constexpr int rows = 10;

                constexpr float spacing = 0.8f;
                constexpr float scale = 0.25f;

                for (int row = 0; row < rows; ++row)
                {
                    for (int column = 0; column < columns; ++column)
                    {
                        const int index =
                            row * columns + column;

                        Entity& entity =
                            m_Scene.CreateEntity(
                                std::string("QueueTest_") +
                                std::to_string(index));

                        entity.GetTransform().SetPosition(
                        {
                            (static_cast<float>(column) -
                                (columns - 1) * 0.5f) * spacing,

                            1.0f +
                                static_cast<float>(row) * spacing,

                            -12.0f
                        });

                        entity.GetTransform().SetScale(
                        {
                            scale,
                            scale,
                            scale
                        });

                        entity.AddComponent<
                            MeshRendererComponent>();

                        entity.AddComponent<
                            MaterialComponent>();

                        entity.GetComponent<
                            MeshRendererComponent>()
                            .SetMesh(sharedMesh);

                        entity.GetComponent<
                            MaterialComponent>()
                            .SetMaterial(
                                index % 2 == 0
                                    ? materialA
                                    : materialB);
                    }
                }

                std::cout
                    << "Render Queue: creadas "
                    << rows * columns
                    << " entidades de prueba\n";
            }
            else
            {
                std::cerr
                    << "Render Queue: faltan recursos "
                    << "para crear la prueba\n";
            }
        }

    Renderer::SetGrayscaleEnabled(
        false);

    Renderer::SetVignetteEnabled(
        false);

    Renderer::SetVignetteStrength(
        1.5f);
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