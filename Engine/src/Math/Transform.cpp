#include "AtlasEngine/Math/Transform.h"

namespace AtlasEngine
{

Transform::Transform()
{
}

void Transform::SetPosition(const Vector3& position)
{
    m_Position = position;
}

void Transform::SetRotation(const Vector3& rotation)
{
    m_Rotation = rotation;
}

void Transform::SetScale(const Vector3& scale)
{
    m_Scale = scale;
}

const Vector3& Transform::GetPosition() const
{
    return m_Position;
}

const Vector3& Transform::GetRotation() const
{
    return m_Rotation;
}

const Vector3& Transform::GetScale() const
{
    return m_Scale;
}

Matrix4 Transform::GetMatrix() const
{
    return
    Matrix4::Translate(
        m_Position.x,
        m_Position.y,
        m_Position.z)

    *

    Matrix4::RotateZ(
        m_Rotation.z)

    *

    Matrix4::Scale(
        m_Scale.x,
        m_Scale.y,
        m_Scale.z);
}

}