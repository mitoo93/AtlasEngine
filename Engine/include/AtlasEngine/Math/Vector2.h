#pragma once

namespace AtlasEngine
{

struct Vector2
{
    float x = 0.0f;
    float y = 0.0f;

    Vector2() = default;

    Vector2(float xValue,
            float yValue)
        : x(xValue),
          y(yValue)
    {
    }
};

}