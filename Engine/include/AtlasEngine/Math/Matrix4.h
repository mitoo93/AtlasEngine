#pragma once
#include "AtlasEngine/Math/Vector3.h"

namespace AtlasEngine
{

class Matrix4
{
public:

    Matrix4();

    static Matrix4 Identity();

    static Matrix4 Translate(
        float x,
        float y,
        float z);

    static Matrix4 Scale(
        float x,
        float y,
        float z);

    static Matrix4 RotateX(
        float angleDegrees);

    static Matrix4 RotateY(
        float angleDegrees);

    static Matrix4 RotateZ(
        float angleDegrees);

    static Matrix4 Perspective(
        float fovDegrees,
        float aspectRatio,
        float nearPlane,
        float farPlane);

    Matrix4 Inverse() const;

    Matrix4 operator*(const Matrix4& other) const;

    float GetTranslationX() const;
    float GetTranslationY() const;
    float GetTranslationZ() const;

    float* Data();
    const float* Data() const;

    Vector3 TransformPoint(
        const Vector3& point) const;

    static Matrix4 Orthographic(
        float left,
        float right,
        float bottom,
        float top,
        float nearPlane,
        float farPlane);

    static Matrix4 LookAt(
        const Vector3& eye,
        const Vector3& target,
        const Vector3& up);

private:

    float m_Values[16];
};

}