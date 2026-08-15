#pragma once

#include <string>

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

private:

    Shader* m_Shader = nullptr;
    Texture* m_Texture = nullptr;

    std::string m_Name;
};

}