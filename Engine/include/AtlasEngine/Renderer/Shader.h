#pragma once

#include "AtlasEngine/Math/Matrix4.h"

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

    bool Load(
        const std::string& vertexPath,
        const std::string& fragmentPath);

    void Bind() const;
    void Unbind() const;

    void SetMatrix4(
    const std::string& name,
    const Matrix4& matrix) const;

private:

    unsigned int CompileShader(unsigned int type,
                               const std::string& source);

private:

    unsigned int m_RendererID = 0;
};

}