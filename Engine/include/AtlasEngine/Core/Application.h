#pragma once

#include "AtlasEngine/Core/Window.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/VertexArray.h"
#include "AtlasEngine/Renderer/VertexBuffer.h"
#include "AtlasEngine/Renderer/Vertex.h"

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
    VertexArray m_VertexArray;
    VertexBuffer m_VertexBuffer;
};

}