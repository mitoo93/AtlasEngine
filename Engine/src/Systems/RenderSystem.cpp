#include "AtlasEngine/Systems/RenderSystem.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Renderer/Texture.h"
#include "AtlasEngine/Renderer/Model.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Renderer/Cubemap.h"
#include "AtlasEngine/Renderer/BRDFLUT.h"

#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/CameraComponent.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"
#include "AtlasEngine/Scene/ModelRendererComponent.h"
#include "AtlasEngine/Scene/DirectionalLightComponent.h"
#include "AtlasEngine/Scene/AmbientLightComponent.h"
#include "AtlasEngine/Scene/PointLightComponent.h"
#include "AtlasEngine/Scene/SpotLightComponent.h"

#include "AtlasEngine/Assets/AssetManager.h"

#include <cmath>
#include <string>
#include <algorithm>
#include <functional>


namespace AtlasEngine
{


// ========================================================
// UPDATE
// ========================================================

void RenderSystem::Update(
    Scene& scene)
{
    Matrix4 view;

    Matrix4 projection;


    // ----------------------------------------------------
    // SHADOW MAP INITIALIZATION
    // ----------------------------------------------------

    if (!m_ShadowMapInitialized)
    {
        if (m_ShadowMap.Create(
                m_ShadowMapResolution,
                m_ShadowMapResolution))
        {
            m_ShadowMapInitialized =
                true;
        }
    }


    Vector3 cameraPosition
    {
        0.0f,
        0.0f,
        0.0f
    };


    // ----------------------------------------------------
    // CAMERA
    // ----------------------------------------------------

    if (m_UseViewProjectionOverride)
    {
        view =
            m_ViewOverride;

        projection =
            m_ProjectionOverride;

        cameraPosition =
            m_CameraPositionOverride;
    }
    else
    {
        CameraComponent* camera =
            scene.FindPrimaryCamera();

        if (!camera)
        {
            return;
        }


        cameraPosition =
            camera->GetCamera()
                .GetPosition();


        view =
            camera->GetCamera()
                .GetViewMatrix();


        projection =
            camera->GetCamera()
                .GetProjectionMatrix();
    }

    // ----------------------------------------------------
    // CAMERA FRUSTUM
    // ----------------------------------------------------

    Matrix4 viewProjection =
        projection *
        view;

    Frustum cameraFrustum;

    cameraFrustum.Update(
        viewProjection);


    // ====================================================
    // LIGHTS
    // ====================================================


    // ----------------------------------------------------
    // DIRECTIONAL LIGHT
    // ----------------------------------------------------

    DirectionalLightComponent* directionalLight =
        nullptr;


    for (const auto& entity :
         scene.GetEntities())
    {
        if (!entity)
        {
            continue;
        }


        if (entity->HasComponent<
                DirectionalLightComponent>())
        {
            directionalLight =
                &entity->GetComponent<
                    DirectionalLightComponent>();

            break;
        }
    }


    // ----------------------------------------------------
    // AMBIENT LIGHT
    // ----------------------------------------------------

    AmbientLightComponent* ambientLight =
        nullptr;


    for (const auto& entity :
         scene.GetEntities())
    {
        if (!entity)
        {
            continue;
        }


        if (entity->HasComponent<
                AmbientLightComponent>())
        {
            ambientLight =
                &entity->GetComponent<
                    AmbientLightComponent>();

            break;
        }
    }


    // ----------------------------------------------------
    // POINT LIGHTS
    // ----------------------------------------------------

    constexpr int MaxPointLights =
        4;


    PointLightComponent*
        pointLights[MaxPointLights]
        {
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };


    Vector3
        pointLightPositions[MaxPointLights];


    int pointLightCount =
        0;


    for (const auto& entity :
         scene.GetEntities())
    {
        if (!entity)
        {
            continue;
        }


        if (!entity->HasComponent<
                PointLightComponent>())
        {
            continue;
        }


        if (pointLightCount >=
            MaxPointLights)
        {
            break;
        }


        pointLights[pointLightCount] =
            &entity->GetComponent<
                PointLightComponent>();


        pointLightPositions[pointLightCount] =
            entity->GetTransform()
                .GetPosition();


        ++pointLightCount;
    }


    // ----------------------------------------------------
    // SPOT LIGHT
    // ----------------------------------------------------

    SpotLightComponent* spotLight =
        nullptr;


    Vector3 spotLightPosition
    {
        0.0f,
        0.0f,
        0.0f
    };


    for (const auto& entity :
         scene.GetEntities())
    {
        if (!entity)
        {
            continue;
        }


        if (entity->HasComponent<
                SpotLightComponent>())
        {
            spotLight =
                &entity->GetComponent<
                    SpotLightComponent>();


            spotLightPosition =
                entity->GetTransform()
                    .GetPosition();


            break;
        }
    }


    // ====================================================
    // LIGHT SPACE MATRIX
    // ====================================================

    Matrix4 lightSpaceMatrix =
        Matrix4::Identity();


    if (directionalLight)
    {
        Vector3 lightDirection =
            directionalLight
                ->GetDirection()
                .Normalized();


        Vector3 lightTarget
        {
            0.0f,
            0.0f,
            0.0f
        };


        Vector3 lightPosition =
            lightTarget -
            lightDirection *
            m_ShadowLightDistance;


        Vector3 lightUp
        {
            0.0f,
            1.0f,
            0.0f
        };


        if (std::fabs(
                Vector3::Dot(
                    lightDirection,
                    lightUp))
            > 0.99f)
        {
            lightUp =
            {
                0.0f,
                0.0f,
                1.0f
            };
        }


        Matrix4 lightView =
            Matrix4::LookAt(
                lightPosition,
                lightTarget,
                lightUp);


        Matrix4 lightProjection =
            Matrix4::Orthographic(
                -m_ShadowOrthoSize,
                m_ShadowOrthoSize,
                -m_ShadowOrthoSize,
                m_ShadowOrthoSize,
                m_ShadowNearPlane,
                m_ShadowFarPlane);


        lightSpaceMatrix =
            lightProjection *
            lightView;
    }


    // ====================================================
    // GLOBAL RENDER RESOURCES
    // ====================================================

    Shader* shadowShader =
        AssetManager::GetShader(
            "Shadow");


    Cubemap* irradianceMap =
        AssetManager::GetCubemap(
            "DefaultIrradiance");


    Cubemap* prefilterMap =
        AssetManager::GetCubemap(
            "DefaultPrefilter");


    BRDFLUT* brdfLUT =
        Renderer::GetBRDFLUT();


        // ====================================================
        // SHADOW DEPTH PASS
        // ====================================================

        if (m_ShadowMapInitialized &&
            shadowShader &&
            directionalLight)
        {
            constexpr bool enableShadowInstancing = true;

            auto& shadowQueue = m_ShadowQueue;

            shadowQueue.clear();

            // Recoge toda la geometria, sin descarte por camara.
            for (const auto& entity : scene.GetEntities())
            {
                if (!entity)
                {
                    continue;
                }

                const bool hasMeshRenderer =
                    entity->HasComponent<MeshRendererComponent>();

                const bool hasModelRenderer =
                    entity->HasComponent<ModelRendererComponent>();

                if (!hasMeshRenderer && !hasModelRenderer)
                {
                    continue;
                }

                const Matrix4 modelMatrix =
                    entity->GetTransform().GetWorldMatrix();

                const auto enqueueShadow = [&](Mesh* mesh)
                {
                    if (!mesh)
                    {
                        return;
                    }

                    ShadowItem item;
                    item.MeshPtr = mesh;
                    item.ModelMatrix = modelMatrix;

                    shadowQueue.push_back(item);
                };

                if (hasMeshRenderer)
                {
                    enqueueShadow(
                        entity->GetComponent<MeshRendererComponent>()
                            .GetMesh());
                }

                if (hasModelRenderer)
                {
                    Model* model =
                        entity->GetComponent<ModelRendererComponent>()
                            .GetModel();

                    if (model)
                    {
                        for (const auto& meshPtr : model->GetMeshes())
                        {
                            enqueueShadow(meshPtr.get());
                        }
                    }
                }
            }

            std::sort(
                shadowQueue.begin(),
                shadowQueue.end(),
                [](const ShadowItem& a, const ShadowItem& b)
                {
                    return std::less<Mesh*>{}(
                        a.MeshPtr,
                        b.MeshPtr);
                });

            m_ShadowMap.BindForWriting();

            shadowShader->Bind();

            shadowShader->SetMatrix4(
                "u_LightSpaceMatrix",
                lightSpaceMatrix);

            shadowShader->SetBool(
                "u_UseInstancing",
                false);

            auto& instanceMatrices =
                m_ShadowInstanceMatrices;

            instanceMatrices.clear();
            std::size_t begin = 0;

            while (begin < shadowQueue.size())
            {
                Mesh* mesh = shadowQueue[begin].MeshPtr;

                std::size_t end = begin + 1;

                while (end < shadowQueue.size() &&
                    shadowQueue[end].MeshPtr == mesh)
                {
                    ++end;
                }

                const std::size_t instanceCount =
                    end - begin;

                bool drawnInstanced = false;

                if (enableShadowInstancing &&
                    instanceCount > 1)
                {
                    instanceMatrices.clear();

                    for (std::size_t i = begin; i < end; ++i)
                    {
                        const float* matrixData =
                            shadowQueue[i].ModelMatrix.Data();

                        instanceMatrices.insert(
                            instanceMatrices.end(),
                            matrixData,
                            matrixData + 16);
                    }

                    shadowShader->SetBool(
                        "u_UseInstancing",
                        true);

                    drawnInstanced =
                        mesh->DrawInstanced(instanceMatrices);

                    shadowShader->SetBool(
                        "u_UseInstancing",
                        false);

                    if (drawnInstanced)
                    {
                        Renderer::SubmitShadowDraw(
                            mesh->GetVertexCount(),
                            mesh->GetIndexCount(),
                            static_cast<uint32_t>(instanceCount));
                    }
                }

                if (!drawnInstanced)
                {
                    for (std::size_t i = begin; i < end; ++i)
                    {
                        shadowShader->SetMatrix4(
                            "u_Model",
                            shadowQueue[i].ModelMatrix);

                        mesh->Draw();

                        Renderer::SubmitShadowDraw(
                            mesh->GetVertexCount(),
                            mesh->GetIndexCount());
                    }
                }

                begin = end;
            }

            shadowShader->SetBool(
                "u_UseInstancing",
                false);

            m_ShadowMap.Unbind();
        }


        // ====================================================
        // MAIN RENDER PASS
        // ====================================================

        // Restaura el framebuffer HDR y su viewport.
        // Es necesario también cuando no hubo shadow pass.
        Renderer::GetHDRFramebuffer().Bind();

        BuildRenderQueue(
            scene,
            cameraFrustum);

        SortRenderQueue();

        ExecuteRenderQueue(
            view,
            projection,
            lightSpaceMatrix,
            cameraPosition,
            directionalLight,
            ambientLight,
            pointLights,
            pointLightPositions,
            pointLightCount,
            spotLight,
            spotLightPosition,
            irradianceMap,
            prefilterMap,
            brdfLUT);


    // ====================================================
    // SKYBOX PASS
    // ====================================================

    Shader* skyboxShader =
        AssetManager::GetShader(
            "Skybox");


    Mesh* skyboxMesh =
        AssetManager::GetMesh(
            "SkyboxCube");


    Cubemap* skyboxCubemap =
        AssetManager::GetCubemap(
            "DefaultSkybox");


    if (skyboxShader &&
        skyboxMesh &&
        skyboxCubemap)
    {
        glDepthFunc(
            GL_LEQUAL);


        skyboxShader->Bind();


        Matrix4 skyboxView =
            view;


        skyboxView.Data()[12] =
            0.0f;

        skyboxView.Data()[13] =
            0.0f;

        skyboxView.Data()[14] =
            0.0f;


        skyboxShader->SetMatrix4(
            "u_View",
            skyboxView);


        skyboxShader->SetMatrix4(
            "u_Projection",
            projection);


        skyboxCubemap->Bind(
            0);


        skyboxShader->SetInt(
            "u_Skybox",
            0);


        Renderer::SubmitSkyboxDraw();

        skyboxMesh->Draw();


        glDepthFunc(
            GL_LESS);
    }
}


// ========================================================
// BIND MATERIAL
// ========================================================

void RenderSystem::BindMaterial(
    Shader& shader,
    Material& material)
{
    // ----------------------------------------------------
    // SCALAR / COLOR VALUES
    // ----------------------------------------------------

    shader.SetVector3(
        "u_BaseColor",
        material.GetBaseColor());


    shader.SetFloat(
        "u_Shininess",
        material.GetShininess());


    shader.SetFloat(
        "u_SpecularStrength",
        material.GetSpecularStrength());


    shader.SetFloat(
        "u_Metallic",
        material.GetMetallic());


    shader.SetFloat(
        "u_Roughness",
        material.GetRoughness());


    shader.SetFloat(
        "u_AO",
        material.GetAmbientOcclusion());


    shader.SetBool(
        "u_PackedMetallicRoughness",
        material.UsesPackedMetallicRoughness());


    // ----------------------------------------------------
    // ALBEDO
    // SLOT 0
    // ----------------------------------------------------

    Texture* albedoTexture =
        material.GetTexture();


    if (albedoTexture)
    {
        albedoTexture->Bind(
            0);


        shader.SetInt(
            "u_Texture",
            0);


        shader.SetBool(
            "u_HasTexture",
            true);
    }
    else
    {
        shader.SetBool(
            "u_HasTexture",
            false);
    }


    // ----------------------------------------------------
    // NORMAL
    // SLOT 1
    // ----------------------------------------------------

    Texture* normalTexture =
        material.GetNormalTexture();


    if (normalTexture)
    {
        normalTexture->Bind(
            1);


        shader.SetInt(
            "u_NormalTexture",
            1);


        shader.SetBool(
            "u_HasNormalTexture",
            true);
    }
    else
    {
        shader.SetBool(
            "u_HasNormalTexture",
            false);
    }


    // ----------------------------------------------------
    // SPECULAR
    // SLOT 2
    // ----------------------------------------------------

    Texture* specularTexture =
        material.GetSpecularTexture();


    if (specularTexture)
    {
        specularTexture->Bind(
            2);


        shader.SetInt(
            "u_SpecularTexture",
            2);


        shader.SetBool(
            "u_HasSpecularTexture",
            true);
    }
    else
    {
        shader.SetBool(
            "u_HasSpecularTexture",
            false);
    }


    // ----------------------------------------------------
    // METALLIC
    // SLOT 3
    // ----------------------------------------------------

    Texture* metallicTexture =
        material.GetMetallicTexture();


    if (metallicTexture)
    {
        metallicTexture->Bind(
            3);


        shader.SetInt(
            "u_MetallicTexture",
            3);


        shader.SetBool(
            "u_HasMetallicTexture",
            true);
    }
    else
    {
        shader.SetBool(
            "u_HasMetallicTexture",
            false);
    }


    // ----------------------------------------------------
    // ROUGHNESS
    // SLOT 4
    // ----------------------------------------------------

    Texture* roughnessTexture =
        material.GetRoughnessTexture();


    if (roughnessTexture)
    {
        roughnessTexture->Bind(
            4);


        shader.SetInt(
            "u_RoughnessTexture",
            4);


        shader.SetBool(
            "u_HasRoughnessTexture",
            true);
    }
    else
    {
        shader.SetBool(
            "u_HasRoughnessTexture",
            false);
    }


    // ----------------------------------------------------
    // AO
    // SLOT 5
    // ----------------------------------------------------

    Texture* aoTexture =
        material.GetAOTexture();


    if (aoTexture)
    {
        aoTexture->Bind(
            5);


        shader.SetInt(
            "u_AOTexture",
            5);


        shader.SetBool(
            "u_HasAOTexture",
            true);
    }
    else
    {
        shader.SetBool(
            "u_HasAOTexture",
            false);
    }
}


// ========================================================
// BIND LIGHTS
// ========================================================

void RenderSystem::BindLights(
    Shader& shader,
    DirectionalLightComponent* directionalLight,
    AmbientLightComponent* ambientLight,
    PointLightComponent* const* pointLights,
    const Vector3* pointLightPositions,
    int pointLightCount,
    SpotLightComponent* spotLight,
    const Vector3& spotLightPosition,
    const Vector3& cameraPosition)
{
    // ----------------------------------------------------
    // CAMERA
    // ----------------------------------------------------

    shader.SetVector3(
        "u_ViewPosition",
        cameraPosition);


    // ----------------------------------------------------
    // DIRECTIONAL LIGHT
    // ----------------------------------------------------

    if (directionalLight)
    {
        shader.SetVector3(
            "u_LightDirection",
            directionalLight
                ->GetDirection());


        shader.SetVector3(
            "u_LightColor",
            directionalLight
                ->GetColor());


        shader.SetFloat(
            "u_LightIntensity",
            directionalLight
                ->GetIntensity());
    }
    else
    {
        shader.SetFloat(
            "u_LightIntensity",
            0.0f);
    }


    // ----------------------------------------------------
    // AMBIENT LIGHT
    // ----------------------------------------------------

    if (ambientLight)
    {
        shader.SetVector3(
            "u_AmbientColor",
            ambientLight
                ->GetColor());


        shader.SetFloat(
            "u_AmbientIntensity",
            ambientLight
                ->GetIntensity());
    }
    else
    {
        shader.SetFloat(
            "u_AmbientIntensity",
            0.0f);
    }


    // ----------------------------------------------------
    // POINT LIGHTS
    // ----------------------------------------------------

    shader.SetInt(
        "u_PointLightCount",
        pointLightCount);


    for (int i = 0;
         i < pointLightCount;
         ++i)
    {
        PointLightComponent* pointLight =
            pointLights[i];


        if (!pointLight)
        {
            continue;
        }


        const std::string prefix =
            "u_PointLights[" +
            std::to_string(i) +
            "].";


        shader.SetVector3(
            prefix + "Position",
            pointLightPositions[i]);


        shader.SetVector3(
            prefix + "Color",
            pointLight->GetColor());


        shader.SetFloat(
            prefix + "Intensity",
            pointLight->GetIntensity());


        shader.SetFloat(
            prefix + "Constant",
            pointLight->GetConstant());


        shader.SetFloat(
            prefix + "Linear",
            pointLight->GetLinear());


        shader.SetFloat(
            prefix + "Quadratic",
            pointLight->GetQuadratic());
    }


    // ----------------------------------------------------
    // SPOT LIGHT
    // ----------------------------------------------------

    if (spotLight)
    {
        shader.SetVector3(
            "u_SpotLightPosition",
            spotLightPosition);


        shader.SetVector3(
            "u_SpotLightDirection",
            spotLight
                ->GetDirection());


        shader.SetVector3(
            "u_SpotLightColor",
            spotLight
                ->GetColor());


        shader.SetFloat(
            "u_SpotLightIntensity",
            spotLight
                ->GetIntensity());


        shader.SetFloat(
            "u_SpotLightInnerCutoff",
            spotLight
                ->GetInnerCutoff());


        shader.SetFloat(
            "u_SpotLightOuterCutoff",
            spotLight
                ->GetOuterCutoff());
    }
    else
    {
        shader.SetFloat(
            "u_SpotLightIntensity",
            0.0f);
    }
}


// ========================================================
// BIND SHADOW + IBL
// ========================================================

void RenderSystem::BindShadowAndIBL(
    Shader& shader,
    Cubemap* irradianceMap,
    Cubemap* prefilterMap,
    BRDFLUT* brdfLUT)
{
    // ----------------------------------------------------
    // SHADOW MAP
    // SLOT 6
    // ----------------------------------------------------

    if (m_ShadowMapInitialized)
    {
        m_ShadowMap.BindTexture(
            6);


        shader.SetInt(
            "u_ShadowMap",
            6);
    }


    // ----------------------------------------------------
    // IRRADIANCE MAP
    // SLOT 8
    // ----------------------------------------------------

    if (irradianceMap)
    {
        irradianceMap->Bind(
            8);


        shader.SetInt(
            "u_IrradianceMap",
            8);


        shader.SetBool(
            "u_HasIrradianceMap",
            true);
    }
    else
    {
        shader.SetBool(
            "u_HasIrradianceMap",
            false);
    }


    // ----------------------------------------------------
    // PREFILTER MAP
    // SLOT 9
    // ----------------------------------------------------

    if (prefilterMap)
    {
        prefilterMap->Bind(
            9);


        shader.SetInt(
            "u_PrefilterMap",
            9);


        shader.SetBool(
            "u_HasPrefilterMap",
            true);
    }
    else
    {
        shader.SetBool(
            "u_HasPrefilterMap",
            false);
    }


    // ----------------------------------------------------
    // BRDF LUT
    // SLOT 10
    // ----------------------------------------------------

    if (brdfLUT)
    {
        brdfLUT->Bind(
            10);


        shader.SetInt(
            "u_BRDFLUT",
            10);


        shader.SetBool(
            "u_HasBRDFLUT",
            true);
    }
    else
    {
        shader.SetBool(
            "u_HasBRDFLUT",
            false);
    }
}


// ========================================================
// VIEW / PROJECTION OVERRIDE
// ========================================================

void RenderSystem::SetViewProjection(
    const Matrix4& view,
    const Matrix4& projection,
    const Vector3& cameraPosition)
{
    m_ViewOverride =
        view;


    m_ProjectionOverride =
        projection;


    m_CameraPositionOverride =
        cameraPosition;


    m_UseViewProjectionOverride =
        true;
}


void RenderSystem::ClearViewProjection()
{
    m_UseViewProjectionOverride =
        false;
}

bool RenderSystem::CalculateWorldBounds(
    const Vector3& localMin,
    const Vector3& localMax,
    const Matrix4& worldMatrix,
    Vector3& worldMin,
    Vector3& worldMax) const
{
    const Vector3 corners[8]
    {
        { localMin.x, localMin.y, localMin.z },
        { localMax.x, localMin.y, localMin.z },
        { localMin.x, localMax.y, localMin.z },
        { localMax.x, localMax.y, localMin.z },
        { localMin.x, localMin.y, localMax.z },
        { localMax.x, localMin.y, localMax.z },
        { localMin.x, localMax.y, localMax.z },
        { localMax.x, localMax.y, localMax.z }
    };

    worldMin = worldMatrix.TransformPoint(corners[0]);
    worldMax = worldMin;

    for (int i = 1; i < 8; ++i)
    {
        const Vector3 worldCorner =
            worldMatrix.TransformPoint(corners[i]);

        worldMin.x = std::min(worldMin.x, worldCorner.x);
        worldMin.y = std::min(worldMin.y, worldCorner.y);
        worldMin.z = std::min(worldMin.z, worldCorner.z);

        worldMax.x = std::max(worldMax.x, worldCorner.x);
        worldMax.y = std::max(worldMax.y, worldCorner.y);
        worldMax.z = std::max(worldMax.z, worldCorner.z);
    }

    return true;
}

void RenderSystem::BuildRenderQueue(
    Scene& scene,
    const Frustum& cameraFrustum)
{
    m_RenderQueue.clear();

    for (const auto& entity : scene.GetEntities())
    {
        if (!entity)
        {
            continue;
        }

        const bool hasMeshRenderer =
            entity->HasComponent<MeshRendererComponent>();

        const bool hasModelRenderer =
            entity->HasComponent<ModelRendererComponent>();

        if (!hasMeshRenderer && !hasModelRenderer)
        {
            continue;
        }

        const Matrix4 modelMatrix =
            entity->GetTransform().GetWorldMatrix();

        const auto isVisible =
            [&](const Vector3& localMin,
                const Vector3& localMax)
            {
                Vector3 worldMin;
                Vector3 worldMax;

                CalculateWorldBounds(
                    localMin,
                    localMax,
                    modelMatrix,
                    worldMin,
                    worldMax);

                return cameraFrustum.IntersectsAABB(
                    worldMin,
                    worldMax);
            };

        const auto enqueue =
            [&](Mesh* mesh, Material* material, Shader* shader)
            {
                RenderItem item;

                item.MeshPtr = mesh;
                item.MaterialPtr = material;
                item.ShaderPtr = shader;
                item.ModelMatrix = modelMatrix;

                m_RenderQueue.push_back(item);
            };

        // ------------------------------------------------
        // MESH RENDERER
        // ------------------------------------------------

        if (hasMeshRenderer)
        {
            const auto processMeshRenderer = [&]()
            {
                Mesh* mesh =
                    entity->GetComponent<MeshRendererComponent>()
                        .GetMesh();

                if (!mesh ||
                    !entity->HasComponent<MaterialComponent>())
                {
                    return;
                }

                Material* material =
                    entity->GetComponent<MaterialComponent>()
                        .GetMaterial();

                if (!material)
                {
                    return;
                }

                Shader* shader = material->GetShader();

                if (!shader)
                {
                    return;
                }

                Renderer::SubmitObject();

                if (!isVisible(
                        mesh->GetBoundsMin(),
                        mesh->GetBoundsMax()))
                {
                    Renderer::SubmitCulledObject();
                    return;
                }

                enqueue(mesh, material, shader);
            };

            processMeshRenderer();
        }

        // ------------------------------------------------
        // MODEL RENDERER
        // ------------------------------------------------

        if (hasModelRenderer)
        {
            const auto processModelRenderer = [&]()
            {
                Model* model =
                    entity->GetComponent<ModelRendererComponent>()
                        .GetModel();

                if (!model)
                {
                    return;
                }

                Renderer::SubmitObject();

                if (!isVisible(
                        model->GetBoundsMin(),
                        model->GetBoundsMax()))
                {
                    Renderer::SubmitCulledObject();
                    return;
                }

                for (const auto& meshPtr : model->GetMeshes())
                {
                    if (!meshPtr)
                    {
                        continue;
                    }

                    Mesh* mesh = meshPtr.get();

                    Material* material = mesh->GetMaterial();

                    if (!material)
                    {
                        continue;
                    }

                    Shader* shader = material->GetShader();

                    if (!shader)
                    {
                        continue;
                    }

                    enqueue(mesh, material, shader);
                }
            };

            processModelRenderer();
        }
    }
}

void RenderSystem::SortRenderQueue()
{
    std::stable_sort(
        m_RenderQueue.begin(),
        m_RenderQueue.end(),
        [](const RenderItem& a, const RenderItem& b)
        {
            if (a.ShaderPtr != b.ShaderPtr)
            {
                return std::less<Shader*>{}(
                    a.ShaderPtr,
                    b.ShaderPtr);
            }

            if (a.MaterialPtr != b.MaterialPtr)
            {
                return std::less<Material*>{}(
                    a.MaterialPtr,
                    b.MaterialPtr);
            }

            return std::less<Mesh*>{}(
                a.MeshPtr,
                b.MeshPtr);
        });
}

void RenderSystem::ExecuteRenderQueue(
    const Matrix4& view,
    const Matrix4& projection,
    const Matrix4& lightSpaceMatrix,
    const Vector3& cameraPosition,
    DirectionalLightComponent* directionalLight,
    AmbientLightComponent* ambientLight,
    PointLightComponent* const* pointLights,
    const Vector3* pointLightPositions,
    int pointLightCount,
    SpotLightComponent* spotLight,
    const Vector3& spotLightPosition,
    Cubemap* irradianceMap,
    Cubemap* prefilterMap,
    BRDFLUT* brdfLUT)
{
    constexpr bool enableInstancing = true;

    Shader* basicShader =
        AssetManager::GetShader("Basic");

    Shader* pbrShader =
        AssetManager::GetShader("PBR");

    Shader* currentShader = nullptr;
    Material* currentMaterial = nullptr;

    auto& instanceMatrices =
        m_SceneInstanceMatrices;

    instanceMatrices.clear();

    std::size_t begin = 0;

    while (begin < m_RenderQueue.size())
    {
        const RenderItem& item =
            m_RenderQueue[begin];

        if (!item.MeshPtr ||
            !item.MaterialPtr ||
            !item.ShaderPtr)
        {
            ++begin;
            continue;
        }

        // Busca elementos consecutivos compatibles.
        std::size_t end = begin + 1;

        while (end < m_RenderQueue.size())
        {
            const RenderItem& next =
                m_RenderQueue[end];

            if (next.MeshPtr != item.MeshPtr ||
                next.MaterialPtr != item.MaterialPtr ||
                next.ShaderPtr != item.ShaderPtr)
            {
                break;
            }

            ++end;
        }

        Shader& shader = *item.ShaderPtr;

        const bool supportsInstancing =
            item.ShaderPtr == basicShader ||
            item.ShaderPtr == pbrShader;

        if (item.ShaderPtr != currentShader)
        {
            shader.Bind();
            Renderer::SubmitSceneShaderBind();

            shader.SetMatrix4(
                "u_View",
                view);

            shader.SetMatrix4(
                "u_Projection",
                projection);

            shader.SetMatrix4(
                "u_LightSpaceMatrix",
                lightSpaceMatrix);

            BindLights(
                shader,
                directionalLight,
                ambientLight,
                pointLights,
                pointLightPositions,
                pointLightCount,
                spotLight,
                spotLightPosition,
                cameraPosition);

            BindShadowAndIBL(
                shader,
                irradianceMap,
                prefilterMap,
                brdfLUT);

            currentShader = item.ShaderPtr;
            currentMaterial = nullptr;
        }

        if (item.MaterialPtr != currentMaterial)
        {
            BindMaterial(
                shader,
                *item.MaterialPtr);

            Renderer::SubmitSceneMaterialBind();

            currentMaterial = item.MaterialPtr;
        }

        // Evita conservar el estado de un dibujo anterior.
        if (supportsInstancing)
        {
            shader.SetBool(
                "u_UseInstancing",
                false);
        }

        const std::size_t instanceCount =
            end - begin;

        bool drawnInstanced = false;

        if (enableInstancing &&
            supportsInstancing &&
            instanceCount > 1)
        {
            instanceMatrices.clear();

            for (std::size_t i = begin; i < end; ++i)
            {
                const float* matrixData =
                    m_RenderQueue[i].ModelMatrix.Data();

                instanceMatrices.insert(
                    instanceMatrices.end(),
                    matrixData,
                    matrixData + 16);
            }

            shader.SetBool(
                "u_UseInstancing",
                true);

            drawnInstanced =
                item.MeshPtr->DrawInstanced(
                    instanceMatrices);

            shader.SetBool(
                "u_UseInstancing",
                false);

            if (drawnInstanced)
            {
                Renderer::SubmitSceneDraw(
                    item.MeshPtr->GetVertexCount(),
                    item.MeshPtr->GetIndexCount(),
                    static_cast<uint32_t>(instanceCount));
            }
        }

        // Dibujo individual para grupos unitarios,
        // Shaders no adaptados o fallo de preparacion.
        if (!drawnInstanced)
        {
            for (std::size_t i = begin; i < end; ++i)
            {
                const RenderItem& single =
                    m_RenderQueue[i];

                shader.SetMatrix4(
                    "u_Model",
                    single.ModelMatrix);

                single.MeshPtr->Draw();

                Renderer::SubmitSceneDraw(
                    single.MeshPtr->GetVertexCount(),
                    single.MeshPtr->GetIndexCount());
            }
        }

        begin = end;
    }
}

}