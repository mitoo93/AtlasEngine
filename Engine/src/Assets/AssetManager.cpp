#include "AtlasEngine/Assets/AssetManager.h"
#include "AtlasEngine/Assets/ModelImporter.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Texture.h"
#include "AtlasEngine/Renderer/Model.h"


namespace AtlasEngine
{

std::unordered_map<
    std::string,
    std::unique_ptr<Shader>>
AssetManager::s_Shaders;

std::unordered_map<
    std::string,
    std::unique_ptr<Material>>
AssetManager::s_Materials;

std::unordered_map<
    std::string,
    std::unique_ptr<Mesh>>
AssetManager::s_Meshes;

std::unordered_map<
    std::string,
    std::unique_ptr<Texture>>
AssetManager::s_Textures;

std::unordered_map<
    std::string,
    std::unique_ptr<Model>>
AssetManager::s_Models;

bool AssetManager::LoadShader(
    const std::string& name,
    const std::string& vertexPath,
    const std::string& fragmentPath)
{
    if (s_Shaders.find(name) != s_Shaders.end())
    {
        return true;
    }

    auto shader =
        std::make_unique<Shader>();

    if (!shader->Load(
        vertexPath,
        fragmentPath))
    {
        return false;
    }

    s_Shaders[name] =
        std::move(shader);

    return true;
}

Shader* AssetManager::GetShader(
    const std::string& name)
{
    auto it =
        s_Shaders.find(name);

    if (it == s_Shaders.end())
    {
        return nullptr;
    }

    return it->second.get();
}

bool AssetManager::LoadMaterial(
    const std::string& name,
    Shader* shader)
{
    if (s_Materials.find(name) != s_Materials.end())
    {
        return true;
    }

    auto material =
        std::make_unique<Material>();

    material->SetName(name);

    material->SetShader(shader);

    s_Materials[name] =
        std::move(material);

    return true;
}

Material* AssetManager::GetMaterial(
    const std::string& name)
{
    auto it =
        s_Materials.find(name);

    if (it == s_Materials.end())
    {
        return nullptr;
    }

    return it->second.get();
}

bool AssetManager::LoadMesh(
    const std::string& name,
    std::unique_ptr<Mesh> mesh)
{
    if (s_Meshes.find(name) != s_Meshes.end())
    {
        return true;
    }

    mesh->SetName(name);

    s_Meshes[name] =
        std::move(mesh);

    return true;
}

Mesh* AssetManager::GetMesh(
    const std::string& name)
{
    auto it =
        s_Meshes.find(name);

    if (it == s_Meshes.end())
    {
        return nullptr;
    }

    return it->second.get();
}

bool AssetManager::LoadTexture(
    const std::string& name,
    const std::string& path)
{
    if (s_Textures.find(name) != s_Textures.end())
    {
        return true;
    }

    auto texture =
        std::make_unique<Texture>();

    if (!texture->Load(path))
    {
        return false;
    }

    texture->SetName(name);

    s_Textures[name] =
        std::move(texture);

    return true;
}

Texture* AssetManager::GetTexture(
    const std::string& name)
{
    auto it =
        s_Textures.find(name);

    if (it == s_Textures.end())
    {
        return nullptr;
    }

    return it->second.get();
}

bool AssetManager::LoadModel(
    const std::string& name,
    const std::string& path)
{
    if (s_Models.find(name) != s_Models.end())
    {
        return true;
    }

    auto model =
        ModelImporter::Import(path);

    if (!model)
    {
        return false;
    }

    s_Models[name] =
        std::move(model);

    return true;
}

Model* AssetManager::GetModel(
    const std::string& name)
{
    auto it =
        s_Models.find(name);

    if (it == s_Models.end())
    {
        return nullptr;
    }

    return it->second.get();
}

}