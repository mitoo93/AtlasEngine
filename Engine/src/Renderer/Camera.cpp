#include "AtlasEngine/Renderer/Camera.h"
#include "AtlasEngine/Math/Math.h"

namespace AtlasEngine
{

void Camera::SetPosition(const Vector3& position)
{
    m_Position = position;
}

void Camera::Translate(float x, float y, float z)
{
    m_Position.x += x;
    m_Position.y += y;
    m_Position.z += z;
}

void Camera::Translate(
    const Vector3& translation)
{
    m_Position += translation;
}

void Camera::SetRotation(const Vector3& rotation)
{
    m_Rotation = rotation;
}

void Camera::Rotate(const Vector3& rotation)
{
    m_Rotation += rotation;
}

void Camera::Rotate(float x, float y, float z)
{
    Rotate(
    {
        x,
        y,
        z
    });
}

const Vector3& Camera::GetPosition() const
{
    return m_Position;
}

const Vector3& Camera::GetRotation() const
{
    return m_Rotation;
}

Matrix4 Camera::GetViewMatrix() const
{
    return
        Matrix4::RotateX(-m_Rotation.x)
        *
        Matrix4::RotateY(-m_Rotation.y)
        *
        Matrix4::RotateZ(-m_Rotation.z)
        *
        Matrix4::Translate(
            -m_Position.x,
            -m_Position.y,
            -m_Position.z);
}

Matrix4 Camera::GetProjectionMatrix() const
{
    return Matrix4::Perspective(
        m_Fov,
        m_AspectRatio,
        m_NearPlane,
        m_FarPlane);
}

void Camera::SetAspectRatio(float aspectRatio)
{
    m_AspectRatio = aspectRatio;
}

Vector3 Camera::GetForward() const
{
    float pitch =
        Math::ToRadians(m_Rotation.x);

    float yaw =
        Math::ToRadians(m_Rotation.y);

    Vector3 forward;

    forward.x =
        std::cos(pitch) * std::sin(yaw);

    forward.y =
        std::sin(pitch);

    forward.z =
        -std::cos(pitch) * std::cos(yaw);

    return forward.Normalized();
}

Vector3 Camera::GetRight() const
{
    Vector3 worldUp
    {
        0.0f,
        1.0f,
        0.0f
    };

    return
        Vector3::Cross(
            GetForward(),
            worldUp)
        .Normalized();
}

Vector3 Camera::GetUp() const
{
    return
        Vector3::Cross(
            GetRight(),
            GetForward())
        .Normalized();
}

}