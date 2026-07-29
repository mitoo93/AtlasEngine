#pragma once

#include <string>

// Forward declaration.
// Evitamos incluir GLFW en el header.
struct GLFWwindow;

namespace AtlasEngine
{

struct WindowProperties
{
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProperties(
        const std::string& title = "Atlas Engine",
        unsigned int width = 1280,
        unsigned int height = 720)
        : Title(title),
          Width(width),
          Height(height)
    {
    }
};

class Window
{
    
public:

    Window();
    ~Window();

    bool Create(const WindowProperties& properties);

    void Update();

    bool ShouldClose() const;

    GLFWwindow* GetNativeWindow() const;

private:

    bool Init();
    bool CreateNativeWindow();

    void DestroyNativeWindow();
    void Shutdown();

private:

    struct WindowData
    {
        WindowProperties Properties;
        GLFWwindow* Handle = nullptr;
    };

    WindowData m_Data;
};

} // namespace AtlasEngine