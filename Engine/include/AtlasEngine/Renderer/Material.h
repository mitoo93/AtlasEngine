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
    std::string m_Name;
};

}