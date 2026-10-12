#include <glad/glad.h>

#include "AtlasEngine/Renderer/Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace
{

std::string ReadFile(
    const std::string& filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        return "";
    }

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
    {
        glDeleteProgram(m_RendererID);
    }
}

unsigned int Shader::CompileShader(
    unsigned int type,
    const std::string& source)
{
    const GLuint shader =
        glCreateShader(type);

    if (!shader)
    {
        std::cerr
            << "No se pudo crear el Shader\n";

        return 0;
    }

    const char* sourcePointer =
        source.c_str();

    glShaderSource(
        shader,
        1,
        &sourcePointer,
        nullptr);

    glCompileShader(shader);

    GLint success = GL_FALSE;

    glGetShaderiv(
        shader,
        GL_COMPILE_STATUS,
        &success);

    if (success != GL_TRUE)
    {
        GLint logLength = 0;

        glGetShaderiv(
            shader,
            GL_INFO_LOG_LENGTH,
            &logLength);

        std::vector<char> message(
            static_cast<std::size_t>(
                logLength > 0 ? logLength : 1),
            '\0');

        if (logLength > 0)
        {
            glGetShaderInfoLog(
                shader,
                logLength,
                nullptr,
                message.data());
        }

        std::cerr
            << "Error compilando "
            << (type == GL_VERTEX_SHADER
                    ? "Vertex"
                    : "Fragment")
            << " Shader:\n"
            << message.data()
            << '\n';

        glDeleteShader(shader);

        return 0;
    }

    return shader;
}

bool Shader::Create(
    const std::string& vertexSource,
    const std::string& fragmentSource)
{
    const GLuint vertexShader =
        CompileShader(
            GL_VERTEX_SHADER,
            vertexSource);

    if (!vertexShader)
    {
        return false;
    }

    const GLuint fragmentShader =
        CompileShader(
            GL_FRAGMENT_SHADER,
            fragmentSource);

    if (!fragmentShader)
    {
        glDeleteShader(vertexShader);
        return false;
    }

    const GLuint newProgram =
        glCreateProgram();

    if (!newProgram)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        std::cerr
            << "No se pudo crear el programa Shader\n";

        return false;
    }

    glAttachShader(
        newProgram,
        vertexShader);

    glAttachShader(
        newProgram,
        fragmentShader);

    glLinkProgram(newProgram);

    GLint linked = GL_FALSE;

    glGetProgramiv(
        newProgram,
        GL_LINK_STATUS,
        &linked);

    // El programa conserva el resultado del enlace.
    // Los objetos de compilacion ya no son necesarios.
    glDetachShader(newProgram, vertexShader);
    glDetachShader(newProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (linked != GL_TRUE)
    {
        GLint logLength = 0;

        glGetProgramiv(
            newProgram,
            GL_INFO_LOG_LENGTH,
            &logLength);

        std::vector<char> message(
            static_cast<std::size_t>(
                logLength > 0 ? logLength : 1),
            '\0');

        if (logLength > 0)
        {
            glGetProgramInfoLog(
                newProgram,
                logLength,
                nullptr,
                message.data());
        }

        std::cerr
            << "Error enlazando programa Shader:\n"
            << message.data()
            << '\n';

        glDeleteProgram(newProgram);

        // Conserva el programa anterior y su cache.
        return false;
    }

    const GLuint previousProgram =
        m_RendererID;

    bool replaceCurrentBinding = false;

    if (previousProgram)
    {
        GLint currentProgram = 0;

        glGetIntegerv(
            GL_CURRENT_PROGRAM,
            &currentProgram);

        replaceCurrentBinding =
            static_cast<GLuint>(currentProgram) ==
            previousProgram;
    }

    m_RendererID = newProgram;

    // Las localizaciones pertenecen al programa anterior.
    m_UniformLocationCache.clear();

    if (replaceCurrentBinding)
    {
        glUseProgram(m_RendererID);
    }

    if (previousProgram)
    {
        glDeleteProgram(previousProgram);
    }

    return true;
}

bool Shader::Load(
    const std::string& vertexPath,
    const std::string& fragmentPath)
{
    const std::string vertexSource =
        ReadFile(vertexPath);

    const std::string fragmentSource =
        ReadFile(fragmentPath);

    bool validSources = true;

    if (vertexSource.empty())
    {
        std::cerr
            << "Shader vacio o no accesible: "
            << vertexPath
            << '\n';

        validSources = false;
    }

    if (fragmentSource.empty())
    {
        std::cerr
            << "Shader vacio o no accesible: "
            << fragmentPath
            << '\n';

        validSources = false;
    }

    if (!validSources)
    {
        return false;
    }

    return Create(
        vertexSource,
        fragmentSource);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

int Shader::GetUniformLocation(
    const std::string& name) const
{
    if (!m_RendererID)
    {
        return -1;
    }

    const auto found =
        m_UniformLocationCache.find(name);

    if (found != m_UniformLocationCache.end())
    {
        return found->second;
    }

    const GLint location =
        glGetUniformLocation(
            m_RendererID,
            name.c_str());

    // Guarda tambien los uniforms ausentes o inactivos.
    m_UniformLocationCache.emplace(
        name,
        location);

    return location;
}

void Shader::SetMatrix4(
    const std::string& name,
    const Matrix4& matrix) const
{
    const GLint location =
        GetUniformLocation(name);

    if (location == -1)
    {
        return;
    }

    glUniformMatrix4fv(
        location,
        1,
        GL_FALSE,
        matrix.Data());
}

void Shader::SetInt(
    const std::string& name,
    int value) const
{
    const GLint location =
        GetUniformLocation(name);

    if (location == -1)
    {
        return;
    }

    glUniform1i(
        location,
        value);
}

void Shader::SetBool(
    const std::string& name,
    bool value) const
{
    SetInt(
        name,
        value ? 1 : 0);
}

void Shader::SetVector3(
    const std::string& name,
    const Vector3& value) const
{
    const GLint location =
        GetUniformLocation(name);

    if (location == -1)
    {
        return;
    }

    glUniform3f(
        location,
        value.x,
        value.y,
        value.z);
}

void Shader::SetFloat(
    const std::string& name,
    float value) const
{
    const GLint location =
        GetUniformLocation(name);

    if (location == -1)
    {
        return;
    }

    glUniform1f(
        location,
        value);
}

}