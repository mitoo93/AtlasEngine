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

    for (int column = 0; column < 4; column++)
    {
        for (int row = 0; row < 4; row++)
        {
            result.m_Values[column * 4 + row] = 0.0f;

            for (int i = 0; i < 4; i++)
            {
                result.m_Values[column * 4 + row] +=
                    m_Values[i * 4 + row] *
                    other.m_Values[column * 4 + i];
            }
        }
    }

    return result;
}


Matrix4 Matrix4::Translate(
    float x,
    float y,
    float z)
{
    Matrix4 matrix = Identity();

    matrix.m_Values[12] = x;
    matrix.m_Values[13] = y;
    matrix.m_Values[14] = z;

    return matrix;
}


Matrix4 Matrix4::Scale(
    float x,
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

    float angle =
        Math::ToRadians(angleDegrees);

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

    float angle =
        Math::ToRadians(angleDegrees);

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

    float angle =
        Math::ToRadians(angleDegrees);

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

    float fovRadians =
        Math::ToRadians(fovDegrees);

    float tanHalfFov =
        std::tan(fovRadians * 0.5f);

    matrix.m_Values[0] =
        1.0f /
        (aspectRatio * tanHalfFov);

    matrix.m_Values[5] =
        1.0f /
        tanHalfFov;

    matrix.m_Values[10] =
        -(farPlane + nearPlane) /
        (farPlane - nearPlane);

    matrix.m_Values[11] = -1.0f;

    matrix.m_Values[14] =
        -(2.0f * farPlane * nearPlane) /
        (farPlane - nearPlane);

    return matrix;
}


// ------------------------------------------------------------
// INVERSE
// ------------------------------------------------------------

Matrix4 Matrix4::Inverse() const
{
    Matrix4 result;

    const float* m = m_Values;
    float* inv = result.m_Values;

    inv[0] =
        m[5]  * m[10] * m[15]
        - m[5]  * m[11] * m[14]
        - m[9]  * m[6]  * m[15]
        + m[9]  * m[7]  * m[14]
        + m[13] * m[6]  * m[11]
        - m[13] * m[7]  * m[10];

    inv[4] =
        -m[4]  * m[10] * m[15]
        + m[4]  * m[11] * m[14]
        + m[8]  * m[6]  * m[15]
        - m[8]  * m[7]  * m[14]
        - m[12] * m[6]  * m[11]
        + m[12] * m[7]  * m[10];

    inv[8] =
        m[4]  * m[9]  * m[15]
        - m[4]  * m[11] * m[13]
        - m[8]  * m[5]  * m[15]
        + m[8]  * m[7]  * m[13]
        + m[12] * m[5]  * m[11]
        - m[12] * m[7]  * m[9];

    inv[12] =
        -m[4]  * m[9]  * m[14]
        + m[4]  * m[10] * m[13]
        + m[8]  * m[5]  * m[14]
        - m[8]  * m[6]  * m[13]
        - m[12] * m[5]  * m[10]
        + m[12] * m[6]  * m[9];

    inv[1] =
        -m[1]  * m[10] * m[15]
        + m[1]  * m[11] * m[14]
        + m[9]  * m[2]  * m[15]
        - m[9]  * m[3]  * m[14]
        - m[13] * m[2]  * m[11]
        + m[13] * m[3]  * m[10];

    inv[5] =
        m[0]  * m[10] * m[15]
        - m[0]  * m[11] * m[14]
        - m[8]  * m[2]  * m[15]
        + m[8]  * m[3]  * m[14]
        + m[12] * m[2]  * m[11]
        - m[12] * m[3]  * m[10];

    inv[9] =
        -m[0]  * m[9]  * m[15]
        + m[0]  * m[11] * m[13]
        + m[8]  * m[1]  * m[15]
        - m[8]  * m[3]  * m[13]
        - m[12] * m[1]  * m[11]
        + m[12] * m[3]  * m[9];

    inv[13] =
        m[0]  * m[9]  * m[14]
        - m[0]  * m[10] * m[13]
        - m[8]  * m[1]  * m[14]
        + m[8]  * m[2]  * m[13]
        + m[12] * m[1]  * m[10]
        - m[12] * m[2]  * m[9];

    inv[2] =
        m[1]  * m[6]  * m[15]
        - m[1]  * m[7]  * m[14]
        - m[5]  * m[2]  * m[15]
        + m[5]  * m[3]  * m[14]
        + m[13] * m[2]  * m[7]
        - m[13] * m[3]  * m[6];

    inv[6] =
        -m[0]  * m[6]  * m[15]
        + m[0]  * m[7]  * m[14]
        + m[4]  * m[2]  * m[15]
        - m[4]  * m[3]  * m[14]
        - m[12] * m[2]  * m[7]
        + m[12] * m[3]  * m[6];

    inv[10] =
        m[0]  * m[5]  * m[15]
        - m[0]  * m[7]  * m[13]
        - m[4]  * m[1]  * m[15]
        + m[4]  * m[3]  * m[13]
        + m[12] * m[1]  * m[7]
        - m[12] * m[3]  * m[5];

    inv[14] =
        -m[0]  * m[5]  * m[14]
        + m[0]  * m[6]  * m[13]
        + m[4]  * m[1]  * m[14]
        - m[4]  * m[2]  * m[13]
        - m[12] * m[1]  * m[6]
        + m[12] * m[2]  * m[5];

    inv[3] =
        -m[1]  * m[6]  * m[11]
        + m[1]  * m[7]  * m[10]
        + m[5]  * m[2]  * m[11]
        - m[5]  * m[3]  * m[10]
        - m[9]  * m[2]  * m[7]
        + m[9]  * m[3]  * m[6];

    inv[7] =
        m[0]  * m[6]  * m[11]
        - m[0]  * m[7]  * m[10]
        - m[4]  * m[2]  * m[11]
        + m[4]  * m[3]  * m[10]
        + m[8]  * m[2]  * m[7]
        - m[8]  * m[3]  * m[6];

    inv[11] =
        -m[0]  * m[5]  * m[11]
        + m[0]  * m[7]  * m[9]
        + m[4]  * m[1]  * m[11]
        - m[4]  * m[3]  * m[9]
        - m[8]  * m[1]  * m[7]
        + m[8]  * m[3]  * m[5];

    inv[15] =
        m[0]  * m[5]  * m[10]
        - m[0]  * m[6]  * m[9]
        - m[4]  * m[1]  * m[10]
        + m[4]  * m[2]  * m[9]
        + m[8]  * m[1]  * m[6]
        - m[8]  * m[2]  * m[5];

    float determinant =
        m[0] * inv[0]
        + m[1] * inv[4]
        + m[2] * inv[8]
        + m[3] * inv[12];

    if (std::fabs(determinant) < 0.000001f)
    {
        return Matrix4::Identity();
    }

    float inverseDeterminant =
        1.0f / determinant;

    for (int i = 0; i < 16; i++)
    {
        inv[i] *= inverseDeterminant;
    }

    return result;
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


float* Matrix4::Data()
{
    return m_Values;
}


const float* Matrix4::Data() const
{
    return m_Values;
}

Vector3 Matrix4::TransformPoint(
    const Vector3& point) const
{
    float x =
        m_Values[0] * point.x +
        m_Values[4] * point.y +
        m_Values[8] * point.z +
        m_Values[12];

    float y =
        m_Values[1] * point.x +
        m_Values[5] * point.y +
        m_Values[9] * point.z +
        m_Values[13];

    float z =
        m_Values[2] * point.x +
        m_Values[6] * point.y +
        m_Values[10] * point.z +
        m_Values[14];

    float w =
        m_Values[3] * point.x +
        m_Values[7] * point.y +
        m_Values[11] * point.z +
        m_Values[15];

    if (std::fabs(w) > 0.000001f)
    {
        x /= w;
        y /= w;
        z /= w;
    }

    return
    {
        x,
        y,
        z
    };
}

Matrix4 Matrix4::Orthographic(
    float left,
    float right,
    float bottom,
    float top,
    float nearPlane,
    float farPlane)
{
    Matrix4 matrix =
        Identity();

    matrix.m_Values[0] =
        2.0f /
        (right - left);

    matrix.m_Values[5] =
        2.0f /
        (top - bottom);

    matrix.m_Values[10] =
        -2.0f /
        (farPlane - nearPlane);

    matrix.m_Values[12] =
        -(right + left) /
        (right - left);

    matrix.m_Values[13] =
        -(top + bottom) /
        (top - bottom);

    matrix.m_Values[14] =
        -(farPlane + nearPlane) /
        (farPlane - nearPlane);

    return matrix;
}

Matrix4 Matrix4::LookAt(
    const Vector3& eye,
    const Vector3& target,
    const Vector3& up)
{
    Vector3 forward =
        (target - eye)
        .Normalized();

    Vector3 right =
        Vector3::Cross(
            forward,
            up)
        .Normalized();

    Vector3 cameraUp =
        Vector3::Cross(
            right,
            forward);

    Matrix4 matrix =
        Identity();

    matrix.m_Values[0] =
        right.x;

    matrix.m_Values[4] =
        right.y;

    matrix.m_Values[8] =
        right.z;

    matrix.m_Values[1] =
        cameraUp.x;

    matrix.m_Values[5] =
        cameraUp.y;

    matrix.m_Values[9] =
        cameraUp.z;

    matrix.m_Values[2] =
        -forward.x;

    matrix.m_Values[6] =
        -forward.y;

    matrix.m_Values[10] =
        -forward.z;

    matrix.m_Values[12] =
        -Vector3::Dot(
            right,
            eye);

    matrix.m_Values[13] =
        -Vector3::Dot(
            cameraUp,
            eye);

    matrix.m_Values[14] =
        Vector3::Dot(
            forward,
            eye);

    return matrix;
}

}