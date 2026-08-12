#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Core/Time.h"
#include "AtlasEngine/Core/Input.h"
#include "AtlasEngine/Core/Layer.h"

#include "AtlasEngine/Events/EventDispatcher.h"
#include "AtlasEngine/Events/WindowEvent.h"

#include "AtlasEngine/Gameplay/GameLayer.h"
#include "AtlasEngine/Editor/EditorLayer.h"

#include "AtlasEngine/Renderer/Renderer.h"

#include <iostream>

namespace AtlasEngine
{

Application* Application::s_Instance = nullptr;

Application::Application()
{
    s_Instance = this;

    WindowProperties props;

    m_Window.SetEventCallback(
        [this](Event& event)
        {
            OnEvent(event);
        });

    m_Window.Create(props);

    Renderer::InitFramebuffer(
        m_Window.GetWidth(),
        m_Window.GetHeight());

    m_ImGuiLayer.OnAttach();

    Input::SetNativeWindow(
        m_Window.GetNativeWindow());

    std::cout << "Application creada\n";

    
}

void Application::Run()
{
    std::cout << "Engine ejecutandose\n";

    GameLayer* game =
        GetLayer<GameLayer>();

    EditorLayer* editor =
        GetLayer<EditorLayer>();

    std::cout << "GameLayer: "
            << (game ? "OK" : "NULL")
            << '\n';

    std::cout << "EditorLayer: "
            << (editor ? "OK" : "NULL")
            << '\n';

    if (game && editor)
    {
        editor->SetContext(&game->GetScene());

        std::cout 
            << "Scene enviada al editor\n";
    }

    while (!m_Window.ShouldClose())
    {
        Time::Update(
            static_cast<float>(glfwGetTime()));

        glfwPollEvents();
        Input::Update();

        // --------------------------------------------------------
        // UPDATE ORDER
        // --------------------------------------------------------
        // Editor first:
        //   - updates EditorCamera
        //   - processes G / T / Y / U / X / F
        //
        // Game second:
        //   - selects the correct render camera
        //   - renders the scene
        // --------------------------------------------------------

        if (editor)
        {
            editor->OnUpdate(
                Time::GetDeltaTime());
        }


        if (game)
        {
            game->OnUpdate(
                Time::GetDeltaTime());
        }

        m_ImGuiLayer.Begin();

        for (Layer* layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }

        m_ImGuiLayer.End();

        if (Input::IsKeyPressed(Key::Escape))
            break;

        m_Window.Update();
    }
}

void Application::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);

    for (auto it = m_LayerStack.end();
        it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);

        if (event.Handled())
        {
            break;
        }
    }

    dispatcher.Dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent& e)
        {
            return OnWindowResize(e);
        });

    dispatcher.Dispatch<WindowCloseEvent>(
        [this](WindowCloseEvent& e)
        {
            return OnWindowClose(e);
        });
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
    /* std::cout
        << "Resize: "
        << event.GetWidth()
        << " x "
        << event.GetHeight()
        << '\n'; */

    return true;
}

bool Application::OnWindowClose(WindowCloseEvent&)
{
    std::cout
        << "Window Close\n";

    return true;
}

void Application::PushLayer(
    Layer* layer)
{
    m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
    std::cout << "PushOverlay: "
              << layer->GetName()
              << '\n';

    m_LayerStack.PushOverlay(layer);
}

Application& Application::Get()
{
    return *s_Instance;
}

}