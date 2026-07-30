#include <GLFW/glfw3.h>

#include "AtlasEngine/Core/Input.h"

namespace AtlasEngine
{

void* Input::s_NativeWindow = nullptr;

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

namespace
{

constexpr int KeyCount = 12;

constexpr Key Keys[KeyCount]
{
    Key::Space,
    Key::Escape,

    Key::W,
    Key::A,
    Key::S,
    Key::D,

    Key::Q,
    Key::E,

    Key::Up,
    Key::Down,
    Key::Left,
    Key::Right
};

bool s_CurrentKeys[KeyCount]{};
bool s_PreviousKeys[KeyCount]{};

}

static int ToKeyIndex(Key key)
{
    for (int i = 0; i < KeyCount; i++)
    {
        if (Keys[i] == key)
        {
            return i;
        }
    }

    return -1;
}

void Input::SetNativeWindow(void* window)
{
    s_NativeWindow = window;
}

void Input::Update()
{
    GLFWwindow* window =
        static_cast<GLFWwindow*>(s_NativeWindow);

    for (int i = 0; i < KeyCount; i++)
    {
        s_PreviousKeys[i] = s_CurrentKeys[i];
    }

    for (int i = 0; i < KeyCount; i++)
    {
        s_CurrentKeys[i] = false;
    }

    for (int i = 0; i < KeyCount; i++)
    {
        s_CurrentKeys[i] =
            glfwGetKey(
                window,
                ToGLFWKey(Keys[i]))
            == GLFW_PRESS;
    }
}

bool Input::IsKeyDown(Key key)
{
    int index = ToKeyIndex(key);

    if (index == -1)
    {
        return false;
    }

    return s_CurrentKeys[index];
}

bool Input::IsKeyPressed(Key key)
{
    int index = ToKeyIndex(key);

    if (index == -1)
    {
        return false;
    }

    return
        s_CurrentKeys[index] &&
        !s_PreviousKeys[index];
}

bool Input::IsKeyReleased(Key key)
{
    int index = ToKeyIndex(key);

    if (index == -1)
    {
        return false;
    }

    return
        !s_CurrentKeys[index] &&
        s_PreviousKeys[index];
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