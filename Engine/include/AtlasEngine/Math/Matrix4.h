#pragma once

namespace AtlasEngine
{

class Matrix4
{
public:

    Matrix4();

    float GetTranslationX() const;
    float GetTranslationY() const;
    float GetTranslationZ() const;

    static Matrix4 Identity();

    Matrix4 operator*(const Matrix4& other) const;

    static Matrix4 Translate(float x,
                         float y,
                         float z);

    static Matrix4 Scale(float x,
                     float y,
                     float z);

    static Matrix4 RotateZ(float angleDegrees);

    static Matrix4 RotateX(float angleDegrees);

    static Matrix4 RotateY(float angleDegrees);

    static Matrix4 Perspective(
        float fovDegrees,
        float aspectRatio,
        float nearPlane,
        float farPlane);

    float* Data();

    const float* Data() const;

private:

    float m_Values[16];
};

}