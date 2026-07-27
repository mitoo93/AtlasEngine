#pragma once

namespace AtlasEngine
{

class Matrix4
{
public:

    Matrix4();

    static Matrix4 Identity();

    Matrix4 operator*(const Matrix4& other) const;

    const float* Data() const
    {
        return m_Values;
    }

    static Matrix4 Translate(float x,
                         float y,
                         float z);

    static Matrix4 Scale(float x,
                     float y,
                     float z);

    static Matrix4 RotateZ(float angleDegrees);

private:

    float m_Values[16];
};

}