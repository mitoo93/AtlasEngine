#include <glad/glad.h>

#include <iostream>

#include "AtlasEngine/Renderer/Shader.h"


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

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

}