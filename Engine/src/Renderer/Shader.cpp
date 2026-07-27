#include <glad/glad.h>

#include <iostream>

#include <fstream>
#include <sstream>

#include "AtlasEngine/Math/Matrix4.h"

#include "AtlasEngine/Renderer/Shader.h"


namespace
{

std::string ReadFile(const std::string& filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open())
        return "";

    std::stringstream stream;

    stream << file.rdbuf();

    return stream.str();
}

}


namespace AtlasEngine
{

Shader::Shader()
{
}

Shader::~Shader()
{
    if (m_RendererID)
        glDeleteProgram(m_RendererID);
}

unsigned int Shader::CompileShader(unsigned int type,
                                   const std::string& source)
{
    GLuint shader = glCreateShader(type);

    const char* src = source.c_str();

    glShaderSource(shader, 1, &src, nullptr);

    glCompileShader(shader);

    GLint success = 0;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint length = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::string message;
        message.resize(length);

        glGetShaderInfoLog(
            shader,
            length,
            &length,
            &message[0]);

        std::cout << "Error compilando "
            << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
            << " Shader:\n"
            << message << '\n';

        glDeleteShader(shader);

        return 0;
    }

    return shader;
}

bool Shader::Create(const std::string& vertexSource,
                    const std::string& fragmentSource)
{
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);

    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    m_RendererID = glCreateProgram();

    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);

    glLinkProgram(m_RendererID);
    glValidateProgram(m_RendererID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool Shader::Load(
    const std::string& vertexPath,
    const std::string& fragmentPath)
{
    std::string vertexSource = ReadFile(vertexPath);
    std::string fragmentSource = ReadFile(fragmentPath);

    if (vertexSource.empty() || fragmentSource.empty())
    {
        if (vertexSource.empty())
        {
            std::cout << "No se pudo leer: "
                    << vertexPath << '\n';
            return false;
        }

        if (fragmentSource.empty())
        {
            std::cout << "No se pudo leer: "
                    << fragmentPath << '\n';
            return false;
        }
        return false;
    }

    return Create(vertexSource, fragmentSource);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetMatrix4(
    const std::string& name,
    const Matrix4& matrix) const
{
    GLint location =
        glGetUniformLocation(
            m_RendererID,
            name.c_str());

    glUniformMatrix4fv(
        location,
        1,
        GL_FALSE,
        matrix.Data());
}

}