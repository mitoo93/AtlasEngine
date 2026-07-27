#include <glad/glad.h>

#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/MeshFactory.h"

#include <iostream>

namespace AtlasEngine
{

Application::Application()
{
    WindowProperties props;

    m_Window.Create(props);

    if (!m_Shader.Load(
        "Assets/Shaders/Basic.vert",
        "Assets/Shaders/Basic.frag"))
    {
        std::cout << "Error creando el shader\n";
    }
    else
    {
        std::cout << "Shader creado\n";
    }

    MeshFactory::CreateQuad(m_Mesh);

    m_Transform.SetPosition(
    {
        0.0f,
        0.0f,
        0.0f
    });

    m_Transform.SetScale({
        0.5f,
        0.5f,
        1.0f
    });

    m_Transform.SetRotation({
        0.0f,
        0.0f,
        45.0f
    });

    std::cout << "Application creada\n";
}

void Application::Run()
{
    std::cout << "Engine ejecutandose\n";

    while (!m_Window.ShouldClose())
    {
        Renderer::BeginFrame();

        m_Shader.Bind();

        m_Shader.SetMatrix4(
            "u_Model",
            m_Transform.GetMatrix());

        m_Mesh.Draw();

        GLenum error = glGetError();

        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL Error: " << error << '\n';
        }

        m_Window.Update();

        Renderer::EndFrame();
    }
}

}