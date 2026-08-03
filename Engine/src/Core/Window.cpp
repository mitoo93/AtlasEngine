    #include <glad/glad.h>
    #include <GLFW/glfw3.h>

    #include "AtlasEngine/Core/Window.h"
    #include "AtlasEngine/Events/WindowEvent.h"

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

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

        glfwSetWindowUserPointer(
            m_Data.Handle,
            &m_Data);

        glfwSetWindowSizeCallback(
            m_Data.Handle,
            [](GLFWwindow* window,
            int width,
            int height)
            {
                WindowData& data =
                    *(WindowData*)glfwGetWindowUserPointer(window);

                data.Properties.Width = width;
                data.Properties.Height = height;

                WindowResizeEvent event(
                    width,
                    height);

                if (data.EventCallback)
                {
                    data.EventCallback(event);
                }
            });

        glfwMakeContextCurrent(m_Data.Handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Error inicializando GLAD\n";
            return false;
        }

        std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << '\n';
        std::cout << "GPU            : " << glGetString(GL_RENDERER) << '\n';
        std::cout << "Vendor         : " << glGetString(GL_VENDOR) << '\n';

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
        glfwSwapBuffers(m_Data.Handle);

        glfwPollEvents();
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Data.Handle);
    }

    GLFWwindow* Window::GetNativeWindow() const
    {
        return m_Data.Handle;
    }

    void Window::SetCursorEnabled(bool enabled)
    {
        glfwSetInputMode(
            m_Data.Handle,
            GLFW_CURSOR,
            enabled
                ? GLFW_CURSOR_NORMAL
                : GLFW_CURSOR_DISABLED);
    }

    void Window::SetEventCallback(
        const EventCallbackFn& callback)
    {
        m_Data.EventCallback = callback;
    }

    } // namespace AtlasEngine