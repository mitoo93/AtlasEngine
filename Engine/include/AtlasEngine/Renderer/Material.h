#pragma once

#include <string>

#include "AtlasEngine/Math/Vector3.h"

namespace AtlasEngine
{

class Shader;
class Texture;

class Material
{
public:

    void SetShader(Shader* shader)
    {
        m_Shader = shader;
    }

    Shader* GetShader()
    {
        return m_Shader;
    }

    const Shader* GetShader() const
    {
        return m_Shader;
    }

    void SetTexture(Texture* texture)
    {
        m_Texture = texture;
    }

    Texture* GetTexture()
    {
        return m_Texture;
    }

    const Texture* GetTexture() const
    {
        return m_Texture;
    }

    void SetName(
        const std::string& name)
    {
        m_Name = name;
    }

    const std::string& GetName() const
    {
        return m_Name;
    }

    void SetBaseColor(
        const Vector3& color)
    {
        m_BaseColor = color;
    }

    const Vector3& GetBaseColor() const
    {
        return m_BaseColor;
    }

private:

    Shader* m_Shader = nullptr;
    Texture* m_Texture = nullptr;

    std::string m_Name;

    Vector3 m_BaseColor
    {
        1.0f,
        1.0f,
        1.0f
    };
};

}