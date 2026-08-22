#pragma once

namespace AtlasEngine
{

class Shader;
class Mesh;

class BRDFLUT
{
public:

    BRDFLUT() = default;

    ~BRDFLUT();


    bool Generate(
        Shader& shader,
        Mesh& quad,
        unsigned int size = 512);


    void Bind(
        unsigned int slot) const;


    unsigned int GetRendererID() const
    {
        return m_RendererID;
    }


private:

    unsigned int m_RendererID =
        0;
};

}