#pragma once

#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Core/LayerStack.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Vertex.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Math/Transform.h"
#include "AtlasEngine/Controllers/CameraController.h"
#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Systems/RenderSystem.h"
#include "AtlasEngine/Events/Event.h"
#include "AtlasEngine/Events/WindowEvent.h"

namespace AtlasEngine
{

class Application
{
public:

    Application();
    ~Application() = default;

    void Run();
    void OnEvent(Event& event);
    bool OnWindowResize(WindowResizeEvent& event);

    bool OnWindowClose(WindowCloseEvent& event);

    void PushLayer(Layer* layer);

    void PushOverlay(Layer* layer);

    static Application& Get();

    Window& GetWindow()
    {
        return m_Window;
    }

    const Window& GetWindow() const
    {
        return m_Window;
    }

private:

    Window m_Window;
    Mesh m_Mesh;
    Transform m_Transform;
    CameraController m_CameraController;
    Scene m_Scene;
    LayerStack m_LayerStack;
    static Application* s_Instance;
    
};

}