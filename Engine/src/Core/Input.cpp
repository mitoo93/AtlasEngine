#include <GLFW/glfw3.h>

#include "AtlasEngine/Core/Input.h"
#include <iostream>

namespace AtlasEngine
{

void* Input::s_NativeWindow = nullptr;

namespace
{

constexpr int KeyCount = 19;

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
    Key::R,
    Key::X,

    Key::T,
    Key::Y,
    Key::U,
    Key::F,
    Key::G,

    Key::Up,
    Key::Down,
    Key::Left,
    Key::Right
};

bool s_CurrentKeys[KeyCount]{};
bool s_PreviousKeys[KeyCount]{};

bool s_CurrentMouseButtons[3]{};
bool s_PreviousMouseButtons[3]{};

int ToGLFWKey(Key key)
{
    switch (key)
    {
        case Key::Space:  return GLFW_KEY_SPACE;
        case Key::Escape: return GLFW_KEY_ESCAPE;

        case Key::W: return GLFW_KEY_W;
        case Key::A: return GLFW_KEY_A;
        case Key::S: return GLFW_KEY_S;
        case Key::D: return GLFW_KEY_D;

        case Key::Q: return GLFW_KEY_Q;
        case Key::E: return GLFW_KEY_E;
        case Key::R: return GLFW_KEY_R;
        case Key::X: return GLFW_KEY_X;

        case Key::T: return GLFW_KEY_T;
        case Key::Y: return GLFW_KEY_Y;
        case Key::U: return GLFW_KEY_U;
        case Key::F: return GLFW_KEY_F;
        case Key::G: return GLFW_KEY_G;

        case Key::Up:    return GLFW_KEY_UP;
        case Key::Down:  return GLFW_KEY_DOWN;
        case Key::Left:  return GLFW_KEY_LEFT;
        case Key::Right: return GLFW_KEY_RIGHT;

        default:
            return GLFW_KEY_UNKNOWN;
    }
}

int ToKeyIndex(Key key)
{
    for (int i = 0; i < KeyCount; ++i)
    {
        if (Keys[i] == key)
            return i;
    }

    return -1;
}

int ToGLFWMouseButton(MouseButton button)
{
    switch (button)
    {
        case MouseButton::Left:
            return GLFW_MOUSE_BUTTON_LEFT;

        case MouseButton::Right:
            return GLFW_MOUSE_BUTTON_RIGHT;

        case MouseButton::Middle:
            return GLFW_MOUSE_BUTTON_MIDDLE;
    }

    return GLFW_MOUSE_BUTTON_LEFT;
}

}

void Input::SetNativeWindow(void* window)
{
    s_NativeWindow = window;
}

void Input::Update()
{
    GLFWwindow* window =
        static_cast<GLFWwindow*>(s_NativeWindow);

    if (!window)
        return;

    glfwPollEvents(); // TEMPORALMENTE AQUÍ PARA PROBAR

    // ----------------------------------------------------
    // KEYS
    // ----------------------------------------------------

    for (int i = 0; i < KeyCount; ++i)
    {
        s_PreviousKeys[i] =
            s_CurrentKeys[i];
    }

    for (int i = 0; i < KeyCount; ++i)
    {
        s_CurrentKeys[i] =
            glfwGetKey(
                window,
                ToGLFWKey(Keys[i]))
            == GLFW_PRESS;
    }

    // ----------------------------------------------------
    // MOUSE BUTTONS
    // ----------------------------------------------------

    for (int i = 0; i < 3; ++i)
    {
        s_PreviousMouseButtons[i] =
            s_CurrentMouseButtons[i];
    }

    for (int i = 0; i < 3; ++i)
    {
        s_CurrentMouseButtons[i] =
            glfwGetMouseButton(
                window,
                GLFW_MOUSE_BUTTON_LEFT + i)
            == GLFW_PRESS;
    }

    std::cout
        << "GLFW W: "
        << glfwGetKey(window, GLFW_KEY_W)
        << " | GLFW A: "
        << glfwGetKey(window, GLFW_KEY_A)
        << " | GLFW S: "
        << glfwGetKey(window, GLFW_KEY_S)
        << " | GLFW D: "
        << glfwGetKey(window, GLFW_KEY_D)
        << "\n";
}

bool Input::IsKeyDown(Key key)
{
    int index = ToKeyIndex(key);

    if (index < 0)
        return false;

    return s_CurrentKeys[index];
}

bool Input::IsKeyPressed(Key key)
{
    int index = ToKeyIndex(key);

    if (index < 0)
        return false;

    return
        s_CurrentKeys[index] &&
        !s_PreviousKeys[index];
}

bool Input::IsKeyReleased(Key key)
{
    int index = ToKeyIndex(key);

    if (index < 0)
        return false;

    return
        !s_CurrentKeys[index] &&
        s_PreviousKeys[index];
}

Vector2 Input::GetMousePosition()
{
    Vector2 position{};

    GLFWwindow* window =
        static_cast<GLFWwindow*>(s_NativeWindow);

    if (!window)
        return position;

    double mouseX = 0.0;
    double mouseY = 0.0;

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

bool Input::IsMouseButtonDown(MouseButton button)
{
    int index =
        static_cast<int>(button);

    if (index < 0 || index >= 3)
        return false;

    return s_CurrentMouseButtons[index];
}

bool Input::IsMouseButtonPressed(MouseButton button)
{
    int index =
        static_cast<int>(button);

    if (index < 0 || index >= 3)
        return false;

    return
        s_CurrentMouseButtons[index] &&
        !s_PreviousMouseButtons[index];
}

bool Input::IsMouseButtonReleased(MouseButton button)
{
    int index =
        static_cast<int>(button);

    if (index < 0 || index >= 3)
        return false;

    return
        !s_CurrentMouseButtons[index] &&
        s_PreviousMouseButtons[index];
}

}