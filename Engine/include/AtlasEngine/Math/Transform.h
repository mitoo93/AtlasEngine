#pragma once

#include "AtlasEngine/Math/Vector3.h"
#include "AtlasEngine/Math/Matrix4.h"

namespace AtlasEngine
{

class Transform
{
public:

    Transform();

    void SetPosition(const Vector3& position);
    void SetRotation(const Vector3& rotation);
    void SetScale(const Vector3& scale);

    const Vector3& GetPosition() const;
    const Vector3& GetRotation() const;
    const Vector3& GetScale() const;

    Matrix4 GetMatrix() const;

private:

    Vector3 m_Position;
    Vector3 m_Rotation;
    Vector3 m_Scale = { 1.0f, 1.0f, 1.0f };
};

}