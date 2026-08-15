#include <fstream>
#include <sstream>
#include <string>

#include "AtlasEngine/Math/Vector3.h"
#include "AtlasEngine/Math/Transform.h"

#include "AtlasEngine/Scene/Entity.h"
#include "AtlasEngine/Scene/Scene.h"

#include "AtlasEngine/Serialization/SceneSerializer.h"

#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"
#include "AtlasEngine/Scene/CameraComponent.h"

#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Renderer/Texture.h"

#include "AtlasEngine/Assets/AssetManager.h"

static AtlasEngine::Vector3 ParseVector3(
    const std::string& text)
{
    std::stringstream ss(text);

    float x, y, z;
    char comma;

    ss >> x >> comma
       >> y >> comma
       >> z;

    return { x, y, z };
}

namespace AtlasEngine
{

bool SceneSerializer::Save(
    const Scene& scene,
    const std::string& filepath)
{
    std::ofstream file(filepath);

    if (!file.is_open())
    {
        return false;
    }

    file << "Scene\n\n";

    for (const auto& entity : scene.GetEntities())
    {
        file << "Entity\n";
        file << "Name="
            << entity->GetName()
            << "\n";

        const Transform& transform =
            entity->GetTransform();

        const auto& position =
            transform.GetPosition();

        const auto& rotation =
            transform.GetRotation();

        const auto& scale =
            transform.GetScale();

        file << "Transform\n";

        file << "Position="
            << position.x << ","
            << position.y << ","
            << position.z << "\n";

        file << "Rotation="
            << rotation.x << ","
            << rotation.y << ","
            << rotation.z << "\n";

        file << "Scale="
            << scale.x << ","
            << scale.y << ","
            << scale.z << "\n\n";

        if (entity->HasComponent<MeshRendererComponent>())
        {
            const MeshRendererComponent& meshRenderer =
                entity->GetComponent<MeshRendererComponent>();

            const Mesh* mesh =
                meshRenderer.GetMesh();

            if (mesh)
            {
                file << "MeshRenderer\n";

                file << "Mesh="
                    << mesh->GetName()
                    << "\n\n";
            }
        }

        if (entity->HasComponent<MaterialComponent>())
        {
            const MaterialComponent& materialComponent =
                entity->GetComponent<MaterialComponent>();

            const Material* material =
                materialComponent.GetMaterial();

            if (material)
            {
                file << "Material\n";

                file << "Material="
                    << material->GetName()
                    << "\n";

                const Texture* texture =
                    material->GetTexture();

                if (texture)
                {
                    file << "Texture="
                        << texture->GetName()
                        << "\n";
                }

                file << "\n";
            }
        }

        if (entity->HasComponent<CameraComponent>())
        {
            const CameraComponent& camera =
                entity->GetComponent<CameraComponent>();

            file << "Camera\n";

            file << "Primary="
                << (camera.IsPrimary() ? 1 : 0)
                << "\n\n";
        }
    }

    return true;
}

bool SceneSerializer::Load(
    Scene& scene,
    const std::string& filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    Entity* currentEntity = nullptr;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        if (line == "Scene")
        {
            continue;
        }

        if (line == "Entity")
        {
            currentEntity =
                &scene.CreateEntity("");

            continue;
        }

        if (line.rfind("Name=", 0) == 0 && currentEntity)
        {
            currentEntity->SetName(
                line.substr(5));

            continue;
        }

        if (line.rfind("Position=", 0) == 0 && currentEntity)
        {
            currentEntity->GetTransform().SetPosition(
                ParseVector3(line.substr(9)));

            continue;
        }

        if (line.rfind("Rotation=", 0) == 0 && currentEntity)
        {
            currentEntity->GetTransform().SetRotation(
                ParseVector3(line.substr(9)));

            continue;
        }

        if (line.rfind("Scale=", 0) == 0 && currentEntity)
        {
            currentEntity->GetTransform().SetScale(
                ParseVector3(line.substr(6)));

            continue;
        }

        if (line == "Material" && currentEntity)
        {
            currentEntity->AddComponent<MaterialComponent>();

            continue;
        }

        if (line.rfind("Texture=", 0) == 0 && currentEntity)
        {
            MaterialComponent& materialComponent =
                currentEntity->GetComponent<MaterialComponent>();

            Material* material =
                materialComponent.GetMaterial();

            if (material)
            {
                material->SetTexture(
                    AssetManager::GetTexture(
                        line.substr(8)));
            }

            continue;
        }

        if (line == "MeshRenderer" && currentEntity)
        {
            currentEntity->AddComponent<MeshRendererComponent>();

            continue;
        }

        if (line.rfind("Mesh=", 0) == 0 && currentEntity)
        {
            MeshRendererComponent& meshRenderer =
                currentEntity->GetComponent<MeshRendererComponent>();

            std::string meshName =
                line.substr(5);

            meshRenderer.SetMesh(
                AssetManager::GetMesh(meshName));

            continue;
        }

        if (line == "Camera" && currentEntity)
        {
            currentEntity->AddComponent<CameraComponent>();

            continue;
        }

        if (line.rfind("Primary=", 0) == 0 && currentEntity)
        {
            CameraComponent& camera =
                currentEntity->GetComponent<CameraComponent>();

            camera.SetPrimary(
                line.substr(8) == "1");

            continue;
        }
    }

    return true;
}

}