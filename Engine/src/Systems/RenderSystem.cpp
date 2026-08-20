#include "AtlasEngine/Systems/RenderSystem.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Renderer/Texture.h"
#include "AtlasEngine/Renderer/Model.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Renderer.h"

#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"
#include "AtlasEngine/Scene/ModelRendererComponent.h"
#include "AtlasEngine/Scene/DirectionalLightComponent.h"
#include "AtlasEngine/Scene/AmbientLightComponent.h"
#include "AtlasEngine/Scene/PointLightComponent.h"
#include "AtlasEngine/Scene/SpotLightComponent.h"

#include "AtlasEngine/Assets/AssetManager.h"

#include <iostream>
#include <cmath>


namespace AtlasEngine
{


void RenderSystem::Update(
    Scene& scene)
{
    Matrix4 view;
    Matrix4 projection;

    if (!m_ShadowMapInitialized)
    {
        if (m_ShadowMap.Create(
                m_ShadowMapResolution,
                m_ShadowMapResolution));
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
    // CAMERA OVERRIDE
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
            return;

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
    // Directional Light
    // ----------------------------------------------------
    
    DirectionalLightComponent* directionalLight =
        nullptr;

    for (const auto& entity :
        scene.GetEntities())
    {
        if (!entity)
            continue;

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
    // LIGHT SPACE MATRIX
    // ----------------------------------------------------

    Matrix4 lightSpaceMatrix =
        Matrix4::Identity();

    if (directionalLight)
    {
        Vector3 lightDirection =
            directionalLight->GetDirection()
                .Normalized();

        Vector3 lightTarget
        {
            0.0f,
            0.0f,
            0.0f
        };

        const float lightDistance =
            m_ShadowLightDistance;

        Vector3 lightPosition =
            lightTarget -
            lightDirection *
            lightDistance;

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

    Shader* shadowShader =
        AssetManager::GetShader("Shadow");

    // ----------------------------------------------------
    // Ambient Light
    // ----------------------------------------------------

    AmbientLightComponent* ambientLight =
        nullptr;

    for (const auto& entity :
        scene.GetEntities())
    {
        if (!entity)
            continue;

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
    // Point Light
    // ----------------------------------------------------

    constexpr int MaxPointLights = 4;

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

    int pointLightCount = 0;

    for (const auto& entity :
        scene.GetEntities())
    {
        if (!entity)
            continue;

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
    // Spot Light
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
            continue;

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

    // ----------------------------------------------------
    // SHADOW DEPTH PASS
    // ----------------------------------------------------

    if (m_ShadowMapInitialized &&
        shadowShader &&
        directionalLight)
    {
        m_ShadowMap.BindForWriting();

        shadowShader->Bind();

        for (const auto& entity :
            scene.GetEntities())
        {
            if (!entity)
                continue;

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
                "u_LightSpaceMatrix",
                lightSpaceMatrix);

            shadowShader->SetMatrix4(
                "u_Model",
                entity->GetTransform()
                    .GetWorldMatrix());

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

            if (hasModelRenderer)
            {
                ModelRendererComponent& modelRenderer =
                    entity->GetComponent<
                        ModelRendererComponent>();

                Model* model =
                    modelRenderer.GetModel();

                if (!model)
                    continue;

                for (const auto& meshPtr :
                    model->GetMeshes())
                {
                    if (!meshPtr)
                        continue;

                    meshPtr->Draw();
                }
            }
        }

        m_ShadowMap.Unbind();

        Renderer::GetFramebuffer()
            .Bind();
    }


    // ----------------------------------------------------
    // RENDER ENTITIES
    // ----------------------------------------------------

    for (const auto& entity :
         scene.GetEntities())
    {
        
        const bool hasMeshRenderer =
            entity->HasComponent<MeshRendererComponent>();

        const bool hasModelRenderer =
            entity->HasComponent<ModelRendererComponent>();

        if (!hasMeshRenderer &&
            !hasModelRenderer)
        {
            continue;
        }

        MaterialComponent& materialComponent =
            entity->GetComponent<MaterialComponent>();


        Material* material =
            materialComponent.GetMaterial();


        if (!material)
            continue;


        Shader* entityShader =
            material->GetShader();


        if (!entityShader)
            continue;

        entityShader->Bind();


        entityShader->SetMatrix4(
            "u_Model",
            entity->GetTransform()
                .GetWorldMatrix());


        entityShader->SetMatrix4(
            "u_View",
            view);


        entityShader->SetMatrix4(
            "u_Projection",
            projection);

        entityShader->SetMatrix4(
            "u_LightSpaceMatrix",
            lightSpaceMatrix);

        Texture* texture =
            material->GetTexture();

        if (texture)
        {
            texture->Bind(0);

            entityShader->SetInt(
                "u_Texture",
                0);

            entityShader->SetBool(
                "u_HasTexture",
                true);
        }
        else
        {
            entityShader->SetBool(
                "u_HasTexture",
                false);
        }


        if (hasMeshRenderer)
        {
            MeshRendererComponent& meshRenderer =
                entity->GetComponent<MeshRendererComponent>();

            Mesh* mesh =
                meshRenderer.GetMesh();

            if (mesh)
            {
                entityShader->SetVector3(
                    "u_BaseColor",
                    material->GetBaseColor());

                entityShader->SetVector3(
                    "u_ViewPosition",
                    cameraPosition);

                entityShader->SetFloat(
                    "u_Shininess",
                    material->GetShininess());

                entityShader->SetFloat(
                    "u_SpecularStrength",
                    material->GetSpecularStrength());

                if (directionalLight)
                {
                    entityShader->SetVector3(
                        "u_LightDirection",
                        directionalLight->GetDirection());

                    entityShader->SetVector3(
                        "u_LightColor",
                        directionalLight->GetColor());

                    entityShader->SetFloat(
                        "u_LightIntensity",
                        directionalLight->GetIntensity());
                }

                if (ambientLight)
                {
                    entityShader->SetVector3(
                        "u_AmbientColor",
                        ambientLight->GetColor());

                    entityShader->SetFloat(
                        "u_AmbientIntensity",
                        ambientLight->GetIntensity());
                }

                entityShader->SetInt(
                    "u_PointLightCount",
                    pointLightCount);

                for (int i = 0;
                    i < pointLightCount;
                    ++i)
                {
                    PointLightComponent* pointLight =
                        pointLights[i];

                    if (!pointLight)
                        continue;

                    const std::string prefix =
                        "u_PointLights[" +
                        std::to_string(i) +
                        "].";

                    entityShader->SetVector3(
                        prefix + "Position",
                        pointLightPositions[i]);

                    entityShader->SetVector3(
                        prefix + "Color",
                        pointLight->GetColor());

                    entityShader->SetFloat(
                        prefix + "Intensity",
                        pointLight->GetIntensity());

                    entityShader->SetFloat(
                        prefix + "Constant",
                        pointLight->GetConstant());

                    entityShader->SetFloat(
                        prefix + "Linear",
                        pointLight->GetLinear());

                    entityShader->SetFloat(
                        prefix + "Quadratic",
                        pointLight->GetQuadratic());
                }

                if (spotLight)
                {
                    entityShader->SetVector3(
                        "u_SpotLightPosition",
                        spotLightPosition);

                    entityShader->SetVector3(
                        "u_SpotLightDirection",
                        spotLight->GetDirection());

                    entityShader->SetVector3(
                        "u_SpotLightColor",
                        spotLight->GetColor());

                    entityShader->SetFloat(
                        "u_SpotLightIntensity",
                        spotLight->GetIntensity());

                    entityShader->SetFloat(
                        "u_SpotLightInnerCutoff",
                        spotLight->GetInnerCutoff());

                    entityShader->SetFloat(
                        "u_SpotLightOuterCutoff",
                        spotLight->GetOuterCutoff());
                }

                entityShader->SetBool(
                    "u_HasNormalTexture",
                    false);

                m_ShadowMap.BindTexture(
                    3);

                entityShader->SetInt(
                    "u_ShadowMap",
                    3);
                
                mesh->Draw();
            }
        }

        if (hasModelRenderer)
        {
            ModelRendererComponent& modelRenderer =
                entity->GetComponent<ModelRendererComponent>();

            Model* model =
                modelRenderer.GetModel();

            if (model)
            {

                for (const auto& meshPtr :
                    model->GetMeshes())
                {

                    if (!meshPtr)
                        continue;

                    Mesh* mesh =
                        meshPtr.get();

                    Material* meshMaterial =
                        mesh->GetMaterial();

                    if (!meshMaterial)
                        continue;

                    Shader* meshShader =
                        meshMaterial->GetShader();

                    if (!meshShader)
                        continue;

                    meshShader->Bind();

                    meshShader->SetMatrix4(
                        "u_Model",
                        entity->GetTransform()
                            .GetWorldMatrix());

                    meshShader->SetMatrix4(
                        "u_View",
                        view);

                    meshShader->SetMatrix4(
                        "u_Projection",
                        projection);

                    meshShader->SetVector3(
                        "u_ViewPosition",
                        cameraPosition);

                    meshShader->SetFloat(
                        "u_Shininess",
                        meshMaterial->GetShininess());

                    meshShader->SetFloat(
                        "u_SpecularStrength",
                        meshMaterial->GetSpecularStrength());

                    if (directionalLight)
                    {
                        meshShader->SetVector3(
                            "u_LightDirection",
                            directionalLight->GetDirection());

                        meshShader->SetVector3(
                            "u_LightColor",
                            directionalLight->GetColor());

                        meshShader->SetFloat(
                            "u_LightIntensity",
                            directionalLight->GetIntensity());
                    }

                    if (ambientLight)
                    {
                        meshShader->SetVector3(
                            "u_AmbientColor",
                            ambientLight->GetColor());

                        meshShader->SetFloat(
                            "u_AmbientIntensity",
                            ambientLight->GetIntensity());
                    }

                    meshShader->SetInt(
                        "u_PointLightCount",
                        pointLightCount);

                    for (int i = 0;
                        i < pointLightCount;
                        ++i)
                    {
                        PointLightComponent* pointLight =
                            pointLights[i];

                        if (!pointLight)
                            continue;

                        const std::string prefix =
                            "u_PointLights[" +
                            std::to_string(i) +
                            "].";

                        meshShader->SetVector3(
                            prefix + "Position",
                            pointLightPositions[i]);

                        meshShader->SetVector3(
                            prefix + "Color",
                            pointLight->GetColor());

                        meshShader->SetFloat(
                            prefix + "Intensity",
                            pointLight->GetIntensity());

                        meshShader->SetFloat(
                            prefix + "Constant",
                            pointLight->GetConstant());

                        meshShader->SetFloat(
                            prefix + "Linear",
                            pointLight->GetLinear());

                        meshShader->SetFloat(
                            prefix + "Quadratic",
                            pointLight->GetQuadratic());
                    }

                    if (spotLight)
                    {
                        meshShader->SetVector3(
                            "u_SpotLightPosition",
                            spotLightPosition);

                        meshShader->SetVector3(
                            "u_SpotLightDirection",
                            spotLight->GetDirection());

                        meshShader->SetVector3(
                            "u_SpotLightColor",
                            spotLight->GetColor());

                        meshShader->SetFloat(
                            "u_SpotLightIntensity",
                            spotLight->GetIntensity());

                        meshShader->SetFloat(
                            "u_SpotLightInnerCutoff",
                            spotLight->GetInnerCutoff());

                        meshShader->SetFloat(
                            "u_SpotLightOuterCutoff",
                            spotLight->GetOuterCutoff());
                    }

                    meshShader->SetVector3(
                        "u_BaseColor",
                        meshMaterial->GetBaseColor());

                    Texture* meshTexture =
                        meshMaterial->GetTexture();

                    if (meshTexture)
                    {
                        meshTexture->Bind(0);

                        meshShader->SetInt(
                            "u_Texture",
                            0);

                        meshShader->SetBool(
                            "u_HasTexture",
                            true);
                    }
                    else
                    {
                        meshShader->SetBool(
                            "u_HasTexture",
                            false);
                    }

                    Texture* normalTexture =
                        meshMaterial->GetNormalTexture();

                    if (normalTexture)
                    {
                        normalTexture->Bind(1);

                        meshShader->SetInt(
                            "u_NormalTexture",
                            1);

                        meshShader->SetBool(
                            "u_HasNormalTexture",
                            true);
                    }
                    else
                    {
                        meshShader->SetBool(
                            "u_HasNormalTexture",
                            false);
                    }

                    Texture* specularTexture =
                        meshMaterial->GetSpecularTexture();

                    if (specularTexture)
                    {
                        specularTexture->Bind(2);

                        meshShader->SetInt(
                            "u_SpecularTexture",
                            2);

                        meshShader->SetBool(
                            "u_HasSpecularTexture",
                            true);
                    }
                    else
                    {
                        meshShader->SetBool(
                            "u_HasSpecularTexture",
                            false);
                    }

                    m_ShadowMap.BindTexture(
                        3);

                    meshShader->SetInt(
                        "u_ShadowMap",
                        3);

                    mesh->Draw();
                }
            }
        }
    }
}


// --------------------------------------------------------
// VIEW / PROJECTION OVERRIDE
// --------------------------------------------------------

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