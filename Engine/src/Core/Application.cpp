#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/MeshFactory.h"
#include "AtlasEngine/Core/Time.h"
#include "AtlasEngine/Core/Input.h"

#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/CameraComponent.h"

#include <iostream>

namespace AtlasEngine
{

Application::Application()
{
    WindowProperties props;

    m_Window.Create(props);

    m_Window.SetCursorEnabled(false);

    Input::SetNativeWindow(
        m_Window.GetNativeWindow());

    
    m_Scene.AddSystem<RenderSystem>();

    //----------------------------------------------------
    // Shader
    //----------------------------------------------------

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

    //----------------------------------------------------
    // Camera Entity
    //----------------------------------------------------

    Entity& camera =
        m_Scene.CreateEntity("MainCamera");

    camera.AddComponent<CameraComponent>();

    CameraComponent& cameraComponent =
        camera.GetComponent<CameraComponent>();
        
    m_Scene.SetActiveCamera(
        &cameraComponent);

    cameraComponent.GetCamera().SetAspectRatio(
        props.Width / (float)props.Height);

    cameraComponent.GetCamera().SetPosition({
        0.0f,
        0.0f,
        0.0f
    });

    cameraComponent.GetCamera().SetRotation({
        0.0f,
        0.0f,
        0.0f
    });

    //----------------------------------------------------
    // Player
    //----------------------------------------------------

    Entity& player =
        m_Scene.CreateEntity("Player");

    player.GetTransform().SetPosition({
        0.0f,
        0.0f,
        -2.0f
    });

    player.GetTransform().SetScale({
        0.5f,
        0.5f,
        1.0f
    });

    player.GetTransform().SetRotation({
        30.0f,
        30.0f,
        30.0f
    });

    player.AddComponent<MeshRendererComponent>();

    MeshRendererComponent& renderer =
        player.GetComponent<MeshRendererComponent>();

    MeshFactory::CreateQuad(
        renderer.GetMesh());

    //----------------------------------------------------
    // Enemy
    //----------------------------------------------------

    m_Scene.CreateEntity("Enemy");

    std::cout << "Application creada\n";
}

void Application::Run()
{
    std::cout << "Engine ejecutandose\n";

    while (!m_Window.ShouldClose())
    {
        Time::Update(
            static_cast<float>(glfwGetTime()));

        Input::Update();

        CameraComponent& camera =
            *m_Scene.GetActiveCamera();

        m_CameraController.Update(
            camera.GetCamera(),
            Time::GetDeltaTime());

        Renderer::BeginFrame();

        m_Shader.Bind();

        m_Shader.SetMatrix4(
            "u_View",
            camera.GetCamera().GetViewMatrix());

        m_Shader.SetMatrix4(
            "u_Projection",
            camera.GetCamera().GetProjectionMatrix());

        m_Scene.Update(
            m_Shader);

        GLenum error = glGetError();

        if (error != GL_NO_ERROR)
        {
            std::cout
                << "OpenGL Error: "
                << error
                << '\n';
        }

        if (Input::IsKeyPressed(Key::Escape))
        {
            break;
        }

        m_Window.Update();

        Renderer::EndFrame();
    }
}

}