#pragma once

namespace AtlasEngine
{

struct Vector3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector3() = default;

    Vector3(float xValue,
            float yValue,
            float zValue)
        : x(xValue),
          y(yValue),
          z(zValue)
    {
    }
};

}