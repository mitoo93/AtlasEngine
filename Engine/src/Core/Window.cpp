#include "AtlasEngine/Core/Window.h"

namespace AtlasEngine
{

	Window::Window()
    {
    }

    Window::~Window()
    {
    }

    bool Window::Create(const WindowProperties& properties)
    {
        m_Data.Properties = properties;

        return true;
    }

    void Window::Update()
    {
    }

    bool Window::ShouldClose() const
    {
        return false;
    }

}