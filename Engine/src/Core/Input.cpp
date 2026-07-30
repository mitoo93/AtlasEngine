#include <GLFW/glfw3.h>

#include "AtlasEngine/Core/Input.h"

namespace AtlasEngine
{

void* Input::s_NativeWindow = nullptr;

void Input::SetNativeWindow(void* window)
{
    s_NativeWindow = window;
}

static int ToGLFWKey(AtlasEngine::Key key)
{
    switch (key)
    {
        case AtlasEngine::Key::Space:  return GLFW_KEY_SPACE;
        case AtlasEngine::Key::Escape: return GLFW_KEY_ESCAPE;

        case AtlasEngine::Key::W: return GLFW_KEY_W;
        case AtlasEngine::Key::A: return GLFW_KEY_A;
        case AtlasEngine::Key::S: return GLFW_KEY_S;
        case AtlasEngine::Key::D: return GLFW_KEY_D;

        case AtlasEngine::Key::Q: return GLFW_KEY_Q;
        case AtlasEngine::Key::E: return GLFW_KEY_E;

        case AtlasEngine::Key::Up:    return GLFW_KEY_UP;
        case AtlasEngine::Key::Down:  return GLFW_KEY_DOWN;
        case AtlasEngine::Key::Left:  return GLFW_KEY_LEFT;
        case AtlasEngine::Key::Right: return GLFW_KEY_RIGHT;

        default:
            return GLFW_KEY_UNKNOWN;
    }
}

bool Input::IsKeyPressed(Key key)
{
    GLFWwindow* window =
        static_cast<GLFWwindow*>(s_NativeWindow);

    return glfwGetKey(
        window,
        ToGLFWKey(key)) == GLFW_PRESS;
}

Vector2 Input::GetMousePosition()
{
    GLFWwindow* window =
        static_cast<GLFWwindow*>(s_NativeWindow);

    Vector2 position;

    double mouseX;
    double mouseY;

    glfwGetCursorPos(
        window,
        &mouseX,
        &mouseY);

    position.x =
        static_cast<float>(mouseX);

    position.y =
        static_cast<float>(mouseY);

    return position;
}

}