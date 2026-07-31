#pragma once

#include "AtlasEngine/Scene/Component.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Shader.h"

namespace AtlasEngine
{

class MeshRendererComponent : public Component
{
public:

    Mesh& GetMesh()
    {
        return m_Mesh;
    }

    Shader& GetShader()
    {
        return m_Shader;
    }

    const Mesh& GetMesh() const
    {
        return m_Mesh;
    }

    const Shader& GetShader() const
    {
        return m_Shader;
    }

private:

    Mesh m_Mesh;
    Shader m_Shader;
};

}