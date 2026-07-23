#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"

#include <iostream>

namespace AtlasEngine
{

Application::Application()
{
    std::cout << "Application creada\n";
}

void Application::Run()
{
    std::cout << "Engine ejecutandose\n";
}

}