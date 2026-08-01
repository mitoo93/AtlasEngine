#pragma once

#include "AtlasEngine/Scene/Component.h"
#include "AtlasEngine/Renderer/Shader.h"

namespace AtlasEngine
{

class MaterialComponent : public Component
{
public:

    Shader& GetShader()
    {
        return m_Shader;
    }

    const Shader& GetShader() const
    {
        return m_Shader;
    }

private:

    Shader m_Shader;
};

}