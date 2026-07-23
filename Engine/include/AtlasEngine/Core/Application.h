#pragma once

#include "AtlasEngine/Core/Window.h"

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
};

}