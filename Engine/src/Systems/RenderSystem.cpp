#include "AtlasEngine/Systems/RenderSystem.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"
#include "AtlasEngine/Renderer/Mesh.h"


namespace AtlasEngine
{

void RenderSystem::Update(
    Scene& scene)
{
    for (const auto& entity : scene.GetEntities())
    {

        CameraComponent* camera =
            scene.FindPrimaryCamera();

        if (!camera)
        {
            return;
        }
            
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
        {
            continue;
        }

        Shader* entityShader =
            material->GetShader();

        if (!entityShader)
        {
            continue;
        }

        entityShader->Bind();

        entityShader->SetMatrix4(
            "u_Model",
            entity->GetTransform().GetWorldMatrix());

        entityShader->SetMatrix4(
            "u_View",
            camera->GetCamera().GetViewMatrix());

        entityShader->SetMatrix4(
            "u_Projection",
            camera->GetCamera().GetProjectionMatrix());

        Mesh* mesh =
            meshRenderer.GetMesh();

        if (!mesh)
        {
            continue;
        }

        mesh->Draw();

    }
}

}