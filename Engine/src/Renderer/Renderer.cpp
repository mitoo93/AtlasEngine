#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Renderer/RenderCommand.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/Entity.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"

namespace AtlasEngine
{

void Renderer::BeginFrame()
{
    RenderCommand::SetClearColor(0.15f, 0.15f, 0.18f, 1.0f);
    RenderCommand::Clear();
}

void Renderer::EndFrame()
{
}

void Renderer::DrawScene(
    const Scene& scene,
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