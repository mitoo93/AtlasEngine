#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "AtlasEngine/Renderer/Mesh.h"

namespace AtlasEngine
{

class Shader;

class Mesh;

class Material;

class Texture;

class Model;

class Cubemap;

class AssetManager
{
public:

    static bool LoadShader(
        const std::string& name,
        const std::string& vertexPath,
        const std::string& fragmentPath);

    static Shader* GetShader(
        const std::string& name);

    static bool LoadMaterial(
        const std::string& name,
        Shader* shader);

    static Material* GetMaterial(
        const std::string& name);

    static bool LoadMesh(
        const std::string& name,
        std::unique_ptr<Mesh> mesh);

    static Mesh* GetMesh(
        const std::string& name);

    static bool LoadTexture(
        const std::string& name,
        const std::string& path);

    static bool LoadTextureFromMemory(
        const std::string& name,
        const unsigned char* data,
        int size);

    static Texture* GetTexture(
        const std::string& name);

    static bool LoadModel(
        const std::string& name,
        const std::string& path);

    static Model* GetModel(
        const std::string& name);

    static bool LoadCubemap(
        const std::string& name,
        const std::vector<std::string>& faces);

    static bool CreateCubemap(
        const std::string& name,
        unsigned int size,
        bool generateMipmaps = false);

    static Cubemap* GetCubemap(
        const std::string& name);

private:

    static std::unordered_map<
        std::string,
        std::unique_ptr<Shader>> s_Shaders;

    static std::unordered_map<
        std::string,
        std::unique_ptr<Material>> s_Materials;

    static std::unordered_map<
        std::string,
        std::unique_ptr<Mesh>> s_Meshes;

    static std::unordered_map<
        std::string,
        std::unique_ptr<Texture>> s_Textures;

    static std::unordered_map<
        std::string,
        std::unique_ptr<Model>> s_Models;

    static std::unordered_map<
        std::string,
        std::unique_ptr<Cubemap>> s_Cubemaps;
    };

}