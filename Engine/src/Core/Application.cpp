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

    m_ImGuiLayer.OnAttach();

    m_Window.SetCursorEnabled(false);

    Input::SetNativeWindow(
        m_Window.GetNativeWindow());

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

        for (Layer* layer : m_LayerStack)
        {
            layer->OnUpdate(Time::GetDeltaTime());
        }

        m_ImGuiLayer.Begin();

        ImGui::ShowDemoWindow();

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

void Application::PushOverlay(
    Layer* layer)
{
    m_LayerStack.PushOverlay(layer);
}

Application& Application::Get()
{
    return *s_Instance;
}

}