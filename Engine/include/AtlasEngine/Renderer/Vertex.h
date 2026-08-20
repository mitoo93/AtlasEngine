#pragma once

#include "AtlasEngine/Renderer/BufferLayout.h"

namespace AtlasEngine
{

struct Vertex
{
    float Position[3];
    float Normal[3];
    float TexCoord[2];

    float Tangent[3];
    float Bitangent[3];

    static BufferLayout GetLayout()
    {
        BufferLayout layout;

        layout.PushFloat(3); // Position
        layout.PushFloat(3); // Normal
        layout.PushFloat(2); // TexCoord
        layout.PushFloat(3); // Tangent
        layout.PushFloat(3); // Bitangent

        return layout;
    }
};
}