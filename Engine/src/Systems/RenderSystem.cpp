#include "AtlasEngine/Systems/RenderSystem.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"

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

        MaterialComponent& material =
            entity->GetComponent<MaterialComponent>();

        Shader& entityShader =
            material.GetShader();

        entityShader.Bind();

        entityShader.SetMatrix4(
            "u_Model",
            entity->GetTransform().GetWorldMatrix());

        entityShader.SetMatrix4(
            "u_View",
            camera->GetCamera().GetViewMatrix());

        entityShader.SetMatrix4(
            "u_Projection",
            camera->GetCamera().GetProjectionMatrix());

        entityShader.SetMatrix4(
            "u_View",
            camera->GetCamera().GetViewMatrix());

        entityShader.SetMatrix4(
            "u_Projection",
            camera->GetCamera().GetProjectionMatrix());

        meshRenderer.GetMesh().Draw();

    }
}

}