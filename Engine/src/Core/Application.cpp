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
        { {  0.0f,  0.5f, 0.0f } },
        { { -0.5f, -0.5f, 0.0f } },
        { {  0.5f, -0.5f, 0.0f } }
    };

    m_VertexArray.Create();

    std::cout << "VAO creado\n";

    m_VertexBuffer.Create(
        vertices,
        sizeof(vertices));

    std::cout << "VBO creado\n";
    
    m_VertexArray.Bind();
    m_VertexBuffer.Bind();

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)0);

    glEnableVertexAttribArray(0);



    std::cout << "Application creada\n";
}

void Application::Run()
{
    std::cout << "Engine ejecutandose\n";

    while (!m_Window.ShouldClose())
    {
        Renderer::BeginFrame();

        m_Shader.Bind();

        m_VertexArray.Bind();

        glDrawArrays(GL_TRIANGLES, 0, 3);

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