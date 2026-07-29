#pragma once

#include "AtlasEngine/Core/Key.h"

namespace AtlasEngine
{

class Input
{
public:

    static bool IsKeyPressed(Key key);

    static void SetNativeWindow(void* window);

private:

    static void* s_NativeWindow;
};

}