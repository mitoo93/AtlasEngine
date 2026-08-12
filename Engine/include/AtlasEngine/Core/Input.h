#pragma once

#include "AtlasEngine/Core/Key.h"
#include "AtlasEngine/Math/Vector2.h"
#include "AtlasEngine/Core/MouseButton.h"

namespace AtlasEngine
{

class Input
{
public:

    static bool IsKeyPressed(Key key);
    static bool IsKeyDown(Key key);
    static bool IsKeyReleased(Key key);

    static bool IsMouseButtonDown(MouseButton button);
    static bool IsMouseButtonPressed(MouseButton button);
    static bool IsMouseButtonReleased(MouseButton button);

    static void SetNativeWindow(void* window);

    static Vector2 GetMousePosition();

    static void Update();

private:

    static void* s_NativeWindow;
};

}