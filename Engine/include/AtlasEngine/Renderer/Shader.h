#pragma once

#include "AtlasEngine/Math/Matrix4.h"
#include "AtlasEngine/Math/Vector3.h"

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

    void SetInt(
        const std::string& name,
        int value) const;

    void SetBool(
        const std::string& name,
        bool value) const;

    void SetVector3(
        const std::string& name,
        const Vector3& value) const;

    void SetFloat(
        const std::string& name,
        float value) const;

private:

    unsigned int CompileShader(unsigned int type,
                               const std::string& source);

private:

    unsigned int m_RendererID = 0;
};

}