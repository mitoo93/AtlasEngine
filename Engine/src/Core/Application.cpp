#include <glad/glad.h>

#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Renderer/Shader.h"

#include <iostream>

namespace AtlasEngine
{

Application::Application()
{
    WindowProperties props;

    m_Window.Create(props);

    const std::string vertexShader = R"(#version 330 core

    layout(location = 0) in vec3 a_Position;

    void main()
    {
        gl_Position = vec4(a_Position, 1.0);
    }
    )";

    const std::string fragmentShader = R"(#version 330 core

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.3, 0.2, 1.0);
    }
    )";

    m_Shader.Create(vertexShader, fragmentShader);

    std::cout << "Shader creado\n";

    Vertex vertices[]
    {
        { { -0.5f, -0.5f, 0.0f } }, // 0
        { {  0.5f, -0.5f, 0.0f } }, // 1
        { {  0.5f,  0.5f, 0.0f } }, // 2
        { { -0.5f,  0.5f, 0.0f } }  // 3
    };

    unsigned int indices[]
    {
        0, 1, 2,
        2, 3, 0
    };

    m_Mesh.Create(
        vertices,
        sizeof(vertices),
        indices,
        6);

    std::cout << "Application creada\n";
}

void Application::Run()
{
    std::cout << "Engine ejecutandose\n";

    while (!m_Window.ShouldClose())
    {
        Renderer::BeginFrame();

        m_Shader.Bind();

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