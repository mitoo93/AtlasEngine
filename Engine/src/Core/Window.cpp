#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "AtlasEngine/Core/Window.h"

#include <iostream>

namespace AtlasEngine
{

Window::Window()
{
}

Window::~Window()
{
    DestroyNativeWindow();
    Shutdown();
}

bool Window::Create(const WindowProperties& properties)
{
    m_Data.Properties = properties;

    if (!Init())
        return false;

    if (!CreateNativeWindow())
        return false;

    return true;
}

bool Window::Init()
{
    if (!glfwInit())
    {
        std::cout << "Error inicializando GLFW\n";
        return false;
    }

    std::cout << "GLFW inicializado correctamente\n";

    return true;
}

bool Window::CreateNativeWindow()
{
    m_Data.Handle = glfwCreateWindow(
        m_Data.Properties.Width,
        m_Data.Properties.Height,
        m_Data.Properties.Title.c_str(),
        nullptr,
        nullptr);

    if (!m_Data.Handle)
    {
        std::cout << "Error creando la ventana\n";

        Shutdown();

        return false;
    }

    glfwMakeContextCurrent(m_Data.Handle);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Error inicializando GLAD\n";
        return false;
    }

    std::cout << "GLAD inicializado correctamente\n";
    std::cout << "Ventana creada correctamente\n";
    return true;
}

void Window::DestroyNativeWindow()
{
    if (m_Data.Handle)
    {
        glfwDestroyWindow(m_Data.Handle);
    }
}

void Window::Shutdown()
{
    glfwTerminate();
}

void Window::Update()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f); //se añade color gris oscuro para diferenciar un poco del negro de la consola
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(m_Data.Handle);

    glfwPollEvents();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_Data.Handle);
}

} // namespace AtlasEngine