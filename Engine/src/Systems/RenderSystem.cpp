#include "AtlasEngine/Systems/RenderSystem.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Renderer/Texture.h"

#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"

#include "AtlasEngine/Renderer/Mesh.h"


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
        if (!entity->HasComponent<MeshRendererComponent>() ||
            !entity->HasComponent<MaterialComponent>())
        {
            continue;
        }


        MeshRendererComponent& meshRenderer =
            entity->GetComponent<MeshRendererComponent>();


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


        Mesh* mesh =
            meshRenderer.GetMesh();


        if (!mesh)
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
        }


        mesh->Draw();
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