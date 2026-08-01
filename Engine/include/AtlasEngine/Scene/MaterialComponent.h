#pragma once

#include "AtlasEngine/Scene/Component.h"

namespace AtlasEngine
{

class Material;

class MaterialComponent : public Component
{
public:

    void SetMaterial(Material* material)
    {
        m_Material = material;
    }

    Material* GetMaterial()
    {
        return m_Material;
    }

    const Material* GetMaterial() const
    {
        return m_Material;
    }

private:

    Material* m_Material = nullptr;
};

}