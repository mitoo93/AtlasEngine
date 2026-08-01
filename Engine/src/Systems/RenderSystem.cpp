#include "AtlasEngine/Systems/RenderSystem.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"

namespace AtlasEngine
{

void RenderSystem::Update(
    Scene& scene,
    Shader& shader)
{
    for (const auto& entity : scene.GetEntities())
    {
        if (!entity->HasComponent<MeshRendererComponent>())
        {
            continue;
        }

        MeshRendererComponent& meshRenderer =
            entity->GetComponent<MeshRendererComponent>();

        shader.SetMatrix4(
            "u_Model",
            entity->GetTransform().GetWorldMatrix());

        meshRenderer.GetMesh().Draw();
    }
}

}