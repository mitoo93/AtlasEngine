#pragma once

#include "AtlasEngine/Math/Vector3.h"

namespace AtlasEngine
{

struct Plane
{
    Vector3 Normal
    {
        0.0f,
        0.0f,
        0.0f
    };

    float Distance =
        0.0f;

    void Normalize();
};

}