#pragma once

#include "AtlasEngine/Renderer/BufferLayout.h"

namespace AtlasEngine
{

struct Vertex
{
    float Position[3];

    static BufferLayout GetLayout()
    {
        BufferLayout layout;
        layout.PushFloat(3);
        return layout;
    }
};

}