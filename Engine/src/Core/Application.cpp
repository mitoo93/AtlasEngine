#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/MeshFactory.h"
#include "AtlasEngine/Core/Time.h"
#include "AtlasEngine/Core/Input.h"

#include <iostream>

namespace AtlasEngine
{

Application::Application()
{
    WindowProperties props;

    m_Window.Create(props);

    Input::SetNativeWindow(
        m_Window.GetNativeWindow());

    m_Camera.SetAspectRatio(
        props.Width / (float)props.Height);

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

    m_Transform.SetPosition({
        0.0f,
        0.0f,
        -2.0f
    });

    m_Transform.SetScale({
        0.5f,
        0.5f,
        1.0f
    });

    m_Transform.SetRotation({
        30.0f,
        30.0f,
        30.0f
    });

    m_Camera.SetPosition({
        0.0f,
        0.0f,
        0.0f
    });

    m_Camera.SetRotation(
    {
        0.0f,
        0.0f,
        0.0f
    });

    std::cout << "Application creada\n";
}

void Application::Run()
{
    std::cout << "Engine ejecutandose\n";

    while (!m_Window.ShouldClose())
    {
        Time::Update(
        static_cast<float>(glfwGetTime()));

        const float cameraSpeed = 2.0f;
        const float rotationSpeed = 90.0f;

        float movement =
            cameraSpeed *
            Time::GetDeltaTime();

        if (Input::IsKeyPressed(Key::W))
        {
            m_Camera.Translate(
                m_Camera.GetForward()
                * movement);
        }

        if (Input::IsKeyPressed(Key::S))
        {
            m_Camera.Translate(
                m_Camera.GetForward()
                * -movement);
        }

        if (Input::IsKeyPressed(Key::D))
        {
            m_Camera.Translate(
                m_Camera.GetRight()
                * movement);
        }

        if (Input::IsKeyPressed(Key::A))
        {
            m_Camera.Translate(
                m_Camera.GetRight()
                * -movement);
        }

        if (Input::IsKeyPressed(Key::Left))
        {
            m_Camera.Rotate(
                0.0f,
                -rotationSpeed * Time::GetDeltaTime(),
                0.0f);
        }

        if (Input::IsKeyPressed(Key::Right))
        {
            m_Camera.Rotate(
                0.0f,
                rotationSpeed * Time::GetDeltaTime(),
                0.0f);
        }

        if (Input::IsKeyPressed(Key::Up))
        {
            m_Camera.Rotate(
                -rotationSpeed * Time::GetDeltaTime(),
                0.0f,
                0.0f);
        }

        if (Input::IsKeyPressed(Key::Down))
        {
            m_Camera.Rotate(
                rotationSpeed * Time::GetDeltaTime(),
                0.0f,
                0.0f);
        }

        m_Transform.Rotate(
            0.0f,
            0.0f,
            90.0f * Time::GetDeltaTime());

        /* static float accumulator = 0.0f;

        accumulator += Time::GetDeltaTime();

        if (accumulator >= 1.0f)
        {
            accumulator = 0.0f;

            std::cout
                << "FPS: "
                << 1.0f / Time::GetDeltaTime()
                << '\n';
        } */
        
        Renderer::BeginFrame();

        m_Shader.Bind();

        m_Shader.SetMatrix4(
            "u_Model",
            m_Transform.GetMatrix());

        m_Shader.SetMatrix4(
            "u_View",
            m_Camera.GetViewMatrix());

        m_Shader.SetMatrix4(
            "u_Projection",
            m_Camera.GetProjectionMatrix());

        m_Mesh.Draw();

        GLenum error = glGetError();

        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL Error: " << error << '\n';
        }

        /* if (Input::IsKeyPressed(Key::Space))
        {
            std::cout << "SPACE\n";
        }

        if (Input::IsKeyPressed(Key::W))
        {
            std::cout << "W\n";
        }

        if (Input::IsKeyPressed(Key::A))
        {
            std::cout << "A\n";
        }

        if (Input::IsKeyPressed(Key::S))
        {
            std::cout << "S\n";
        }

        if (Input::IsKeyPressed(Key::D))
        {
            std::cout << "D\n";
        } */

        m_Window.Update();

        Renderer::EndFrame();
    }
}

}