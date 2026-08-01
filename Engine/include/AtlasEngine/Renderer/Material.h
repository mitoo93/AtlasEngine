#pragma once

namespace AtlasEngine
{

class Shader;

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

private:

    Shader* m_Shader = nullptr;
};

}