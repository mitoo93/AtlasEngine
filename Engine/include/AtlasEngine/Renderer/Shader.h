#pragma once

#include <string>

namespace AtlasEngine
{

class Shader
{
public:

    Shader();
    ~Shader();

    bool Create(const std::string& vertexSource,
                const std::string& fragmentSource);

    void Bind() const;
    void Unbind() const;

private:

    unsigned int CompileShader(unsigned int type,
                               const std::string& source);

private:

    unsigned int m_RendererID = 0;
};

}