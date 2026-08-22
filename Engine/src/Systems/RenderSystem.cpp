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
        m_ShadowMap.BindForWriting();


        shadowShader->Bind();


        shadowShader->SetMatrix4(
            "u_LightSpaceMatrix",
            lightSpaceMatrix);


        for (const auto& entity :
             scene.GetEntities())
        {
            if (!entity)
            {
                continue;
            }


            const bool hasMeshRenderer =
                entity->HasComponent<
                    MeshRendererComponent>();


            const bool hasModelRenderer =
                entity->HasComponent<
                    ModelRendererComponent>();


            if (!hasMeshRenderer &&
                !hasModelRenderer)
            {
                continue;
            }


            shadowShader->SetMatrix4(
                "u_Model",
                entity->GetTransform()
                    .GetWorldMatrix());


            // --------------------------------------------
            // MESH
            // --------------------------------------------

            if (hasMeshRenderer)
            {
                MeshRendererComponent& meshRenderer =
                    entity->GetComponent<
                        MeshRendererComponent>();


                Mesh* mesh =
                    meshRenderer.GetMesh();


                if (mesh)
                {
                    mesh->Draw();
                }
            }


            // --------------------------------------------
            // MODEL
            // --------------------------------------------

            if (hasModelRenderer)
            {
                ModelRendererComponent& modelRenderer =
                    entity->GetComponent<
                        ModelRendererComponent>();


                Model* model =
                    modelRenderer.GetModel();


                if (!model)
                {
                    continue;
                }


                for (const auto& meshPtr :
                     model->GetMeshes())
                {
                    if (!meshPtr)
                    {
                        continue;
                    }


                    meshPtr->Draw();
                }
            }
        }


        m_ShadowMap.Unbind();


        Renderer::GetHDRFramebuffer()
            .Bind();
    }


    // ====================================================
    // MAIN RENDER PASS
    // ====================================================

    for (const auto& entity :
         scene.GetEntities())
    {
        if (!entity)
        {
            continue;
        }


        const bool hasMeshRenderer =
            entity->HasComponent<
                MeshRendererComponent>();


        const bool hasModelRenderer =
            entity->HasComponent<
                ModelRendererComponent>();


        if (!hasMeshRenderer &&
            !hasModelRenderer)
        {
            continue;
        }


        const Matrix4 modelMatrix =
            entity->GetTransform()
                .GetWorldMatrix();


        // =================================================
        // MESH RENDERER
        // =================================================

        if (hasMeshRenderer)
        {
            if (!entity->HasComponent<
                    MaterialComponent>())
            {
                continue;
            }


            MeshRendererComponent& meshRenderer =
                entity->GetComponent<
                    MeshRendererComponent>();


            Mesh* mesh =
                meshRenderer.GetMesh();


            if (!mesh)
            {
                continue;
            }


            MaterialComponent& materialComponent =
                entity->GetComponent<
                    MaterialComponent>();


            Material* material =
                materialComponent.GetMaterial();


            if (!material)
            {
                continue;
            }


            Shader* shader =
                material->GetShader();


            if (!shader)
            {
                continue;
            }


            shader->Bind();


            // --------------------------------------------
            // TRANSFORMS
            // --------------------------------------------

            shader->SetMatrix4(
                "u_Model",
                modelMatrix);


            shader->SetMatrix4(
                "u_View",
                view);


            shader->SetMatrix4(
                "u_Projection",
                projection);


            shader->SetMatrix4(
                "u_LightSpaceMatrix",
                lightSpaceMatrix);


            // --------------------------------------------
            // MATERIAL
            // --------------------------------------------

            BindMaterial(
                *shader,
                *material);


            // --------------------------------------------
            // LIGHTS
            // --------------------------------------------

            BindLights(
                *shader,
                directionalLight,
                ambientLight,
                pointLights,
                pointLightPositions,
                pointLightCount,
                spotLight,
                spotLightPosition,
                cameraPosition);


            // --------------------------------------------
            // SHADOW + IBL
            // --------------------------------------------

            BindShadowAndIBL(
                *shader,
                irradianceMap,
                prefilterMap,
                brdfLUT);


            // --------------------------------------------
            // DRAW
            // --------------------------------------------

            mesh->Draw();
        }


        // =================================================
        // MODEL RENDERER
        // =================================================

        if (hasModelRenderer)
        {
            ModelRendererComponent& modelRenderer =
                entity->GetComponent<
                    ModelRendererComponent>();


            Model* model =
                modelRenderer.GetModel();


            if (!model)
            {
                continue;
            }


            for (const auto& meshPtr :
                 model->GetMeshes())
            {
                if (!meshPtr)
                {
                    continue;
                }


                Mesh* mesh =
                    meshPtr.get();


                Material* material =
                    mesh->GetMaterial();


                if (!material)
                {
                    continue;
                }


                Shader* shader =
                    material->GetShader();


                if (!shader)
                {
                    continue;
                }


                shader->Bind();


                // ----------------------------------------
                // TRANSFORMS
                // ----------------------------------------

                shader->SetMatrix4(
                    "u_Model",
                    modelMatrix);


                shader->SetMatrix4(
                    "u_View",
                    view);


                shader->SetMatrix4(
                    "u_Projection",
                    projection);


                shader->SetMatrix4(
                    "u_LightSpaceMatrix",
                    lightSpaceMatrix);


                // ----------------------------------------
                // MATERIAL
                // ----------------------------------------

                BindMaterial(
                    *shader,
                    *material);


                // ----------------------------------------
                // LIGHTS
                // ----------------------------------------

                BindLights(
                    *shader,
                    directionalLight,
                    ambientLight,
                    pointLights,
                    pointLightPositions,
                    pointLightCount,
                    spotLight,
                    spotLightPosition,
                    cameraPosition);


                // ----------------------------------------
                // SHADOW + IBL
                // ----------------------------------------

                BindShadowAndIBL(
                    *shader,
                    irradianceMap,
                    prefilterMap,
                    brdfLUT);


                // ----------------------------------------
                // DRAW
                // ----------------------------------------

                mesh->Draw();
            }
        }
    }


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


}