#pragma once

#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Vertex.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Math/Transform.h"
#include "AtlasEngine/Controllers/CameraController.h"
#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Systems/RenderSystem.h"

namespace AtlasEngine
{

class Application
{
public:

    Application();
    ~Application() = default;

    void Run();

private:

    Window m_Window;
    Shader m_Shader;
    Mesh m_Mesh;
    Transform m_Transform;
    CameraController m_CameraController;
    Scene m_Scene;
    
};

}