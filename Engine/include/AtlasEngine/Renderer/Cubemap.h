#pragma once

#include <string>
#include <vector>

namespace AtlasEngine
{

class Shader;
class Mesh;

class Cubemap
{
public:

    Cubemap();

    ~Cubemap();

    Cubemap(
        const Cubemap&) = delete;

    Cubemap& operator=(
        const Cubemap&) = delete;

    Cubemap(
        Cubemap&&) = delete;

    Cubemap& operator=(
        Cubemap&&) = delete;


    bool Load(
        const std::vector<std::string>& faces);

    void Bind(
        unsigned int slot = 0) const;

    void Unbind() const;


    void SetName(
        const std::string& name);

    const std::string& GetName() const;

    bool Create(
        unsigned int size,
        bool generateMipmaps = false);

    bool GenerateIrradianceMap(
        Cubemap& environmentMap,
        Shader& irradianceShader,
        Mesh& cubeMesh,
        unsigned int size = 32);

    unsigned int GetRendererID() const
    {
        return m_RendererID;
    }

    bool GeneratePrefilterMap(
        Cubemap& environmentMap,
        Shader& prefilterShader,
        Mesh& cubeMesh,
        unsigned int baseSize = 128,
        unsigned int mipLevels = 5);


private:

    unsigned int m_RendererID = 0;

    std::string m_Name;
};

}