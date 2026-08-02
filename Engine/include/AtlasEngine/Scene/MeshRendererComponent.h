#pragma once

#include "AtlasEngine/Scene/Component.h"

namespace AtlasEngine
{

class Mesh;

class MeshRendererComponent : public Component
{
public:

    void SetMesh(Mesh* mesh)
    {
        m_Mesh = mesh;
    }

    Mesh* GetMesh()
    {
        return m_Mesh;
    }

    const Mesh* GetMesh() const
    {
        return m_Mesh;
    }

private:

    Mesh* m_Mesh = nullptr;
};

}