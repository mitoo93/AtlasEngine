#pragma once

#include "AtlasEngine/Renderer/BufferLayout.h"

namespace AtlasEngine
{

struct Vertex
{
    float Position[3];
    float TexCoord[2];

    static BufferLayout GetLayout()
    {
        BufferLayout layout;

        layout.PushFloat(3);
        layout.PushFloat(2);

        return layout;
    }
};

}