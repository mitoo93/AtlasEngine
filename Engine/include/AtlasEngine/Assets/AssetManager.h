#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace AtlasEngine
{

class Shader;

class Material;

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

private:

    static std::unordered_map<
        std::string,
        std::unique_ptr<Shader>> s_Shaders;

    static std::unordered_map<
        std::string,
        std::unique_ptr<Material>> s_Materials;
};

}