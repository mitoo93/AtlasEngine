#include <algorithm>

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

Transform* Transform::GetParent() const
{
    return m_Parent;
}

void Transform::SetParent(Transform* parent)
{
    if (m_Parent)
    {
        auto it =
            std::find(
                m_Parent->m_Children.begin(),
                m_Parent->m_Children.end(),
                this);

        if (it != m_Parent->m_Children.end())
        {
            m_Parent->m_Children.erase(it);
        }
    }

    m_Parent = parent;

    if (m_Parent)
    {
        m_Parent->m_Children.push_back(this);
    }
}

Matrix4 Transform::GetLocalMatrix() const
{
    return
        Matrix4::Translate(
            m_Position.x,
            m_Position.y,
            m_Position.z)
        *
        Matrix4::RotateZ(m_Rotation.z)
        *
        Matrix4::RotateY(m_Rotation.y)
        *
        Matrix4::RotateX(m_Rotation.x)
        *
        Matrix4::Scale(
            m_Scale.x,
            m_Scale.y,
            m_Scale.z);
}

Matrix4 Transform::GetWorldMatrix() const
{
    if (m_Parent)
    {
        return
            m_Parent->GetWorldMatrix()
            *
            GetLocalMatrix();
    }

    return GetLocalMatrix();
}

void Transform::Translate(float x, float y, float z)
{
    m_Position.x += x;
    m_Position.y += y;
    m_Position.z += z;
}

void Transform::Rotate(float x, float y, float z)
{
    m_Rotation.x += x;
    m_Rotation.y += y;
    m_Rotation.z += z;
}

void Transform::Scale(float x, float y, float z)
{
    m_Scale.x += x;
    m_Scale.y += y;
    m_Scale.z += z;
}

}