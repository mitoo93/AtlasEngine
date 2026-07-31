#include <cmath>

#include "AtlasEngine/Math/Math.h"

#include "AtlasEngine/Math/Matrix4.h"

namespace AtlasEngine
{

Matrix4::Matrix4()
{
    for (int i = 0; i < 16; i++)
        m_Values[i] = 0.0f;
}

Matrix4 Matrix4::Identity()
{
    Matrix4 matrix;

    matrix.m_Values[0]  = 1.0f;
    matrix.m_Values[5]  = 1.0f;
    matrix.m_Values[10] = 1.0f;
    matrix.m_Values[15] = 1.0f;

    return matrix;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const
{
    Matrix4 result;

    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            result.m_Values[row * 4 + column] = 0.0f;

            for (int i = 0; i < 4; i++)
            {
                result.m_Values[row * 4 + column] +=
                    m_Values[row * 4 + i] *
                    other.m_Values[i * 4 + column];
            }
        }
    }

    return result;
}

Matrix4 Matrix4::Translate(float x,
                           float y,
                           float z)
{
    Matrix4 matrix = Identity();

    matrix.m_Values[12] = x;
    matrix.m_Values[13] = y;
    matrix.m_Values[14] = z;

    return matrix;
}

Matrix4 Matrix4::Scale(float x,
                       float y,
                       float z)
{
    Matrix4 matrix = Identity();

    matrix.m_Values[0] = x;
    matrix.m_Values[5] = y;
    matrix.m_Values[10] = z;

    return matrix;
}

Matrix4 Matrix4::RotateZ(float angleDegrees)
{
    Matrix4 matrix = Identity();

    float angle = Math::ToRadians(angleDegrees);

    float c = std::cos(angle);
    float s = std::sin(angle);

    matrix.m_Values[0] = c;
    matrix.m_Values[1] = s;

    matrix.m_Values[4] = -s;
    matrix.m_Values[5] = c;

    return matrix;
}

Matrix4 Matrix4::RotateX(float angleDegrees)
{
    Matrix4 matrix = Identity();

    float angle = Math::ToRadians(angleDegrees);

    float c = std::cos(angle);
    float s = std::sin(angle);

    matrix.m_Values[5] = c;
    matrix.m_Values[6] = s;

    matrix.m_Values[9] = -s;
    matrix.m_Values[10] = c;

    return matrix;
}

Matrix4 Matrix4::RotateY(float angleDegrees)
{
    Matrix4 matrix = Identity();

    float angle = Math::ToRadians(angleDegrees);

    float c = std::cos(angle);
    float s = std::sin(angle);

    matrix.m_Values[0] = c;
    matrix.m_Values[2] = -s;

    matrix.m_Values[8] = s;
    matrix.m_Values[10] = c;

    return matrix;
}

Matrix4 Matrix4::Perspective(
    float fovDegrees,
    float aspectRatio,
    float nearPlane,
    float farPlane)
{
    Matrix4 matrix;

    float fovRadians = Math::ToRadians(fovDegrees);

    float tanHalfFov = std::tan(fovRadians * 0.5f);

    matrix.m_Values[0] = 1.0f / (aspectRatio * tanHalfFov);

    matrix.m_Values[5] = 1.0f / tanHalfFov;

    matrix.m_Values[10] =
        -(farPlane + nearPlane) /
        (farPlane - nearPlane);

    matrix.m_Values[11] = -1.0f;

    matrix.m_Values[14] =
        -(2.0f * farPlane * nearPlane) /
        (farPlane - nearPlane);

    return matrix;
}

float Matrix4::GetTranslationX() const
{
    return m_Values[12];
}

float Matrix4::GetTranslationY() const
{
    return m_Values[13];
}

float Matrix4::GetTranslationZ() const
{
    return m_Values[14];
}

}