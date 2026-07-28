#include "AtlasEngine/Renderer/Camera.h"

namespace AtlasEngine
{

void Camera::SetPosition(const Vector3& position)
{
    m_Position = position;
}

void Camera::SetRotation(const Vector3& rotation)
{
    m_Rotation = rotation;
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

}