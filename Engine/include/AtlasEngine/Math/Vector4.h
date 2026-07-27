#pragma once

namespace AtlasEngine
{

struct Vector4
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    Vector4() = default;

    Vector4(float xValue,
            float yValue,
            float zValue,
            float wValue)
        : x(xValue),
          y(yValue),
          z(zValue),
          w(wValue)
    {
    }
};

}