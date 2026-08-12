#pragma once

namespace AtlasEngine
{

class Math
{
public:

    static constexpr float PI =
        3.14159265358979323846f;

    static float ToRadians(float degrees)
    {
        return degrees * PI / 180.0f;
    }

    static float ToDegrees(float radians)
    {
        return radians * 180.0f / PI;
    }
};

}