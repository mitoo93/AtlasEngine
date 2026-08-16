#include "AtlasEngine/Systems/RenderSystem.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Renderer/Texture.h"
#include "AtlasEngine/Renderer/Model.h"
#include "AtlasEngine/Renderer/Mesh.h"

#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"
#include "AtlasEngine/Scene/ModelRendererComponent.h"

#include "AtlasEngine/Renderer/Mesh.h"

#include <iostream>


namespace AtlasEngine
{


void RenderSystem::Update(
    Scene& scene)
{
    Matrix4 view;
    Matrix4 projection;


    // ----------------------------------------------------
    // CAMERA OVERRIDE
    // ----------------------------------------------------

    if (m_UseViewProjectionOverride)
    {
        view =
            m_ViewOverride;

        projection =
            m_ProjectionOverride;
    }
    else
    {
        CameraComponent* camera =
            scene.FindPrimaryCamera();

        if (!camera)
            return;

        view =
            camera->GetCamera()
                .GetViewMatrix();

        projection =
            camera->GetCamera()
                .GetProjectionMatrix();
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
    const Matrix4& projection)
{
    m_ViewOverride =
        view;

    m_ProjectionOverride =
        projection;

    m_UseViewProjectionOverride =
        true;
}


void RenderSystem::ClearViewProjection()
{
    m_UseViewProjectionOverride =
        false;
}


}