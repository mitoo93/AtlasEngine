#pragma once

#include <vector>

#include "AtlasEngine/Math/Matrix4.h"
#include "AtlasEngine/Math/Vector3.h"

namespace AtlasEngine
{

class Transform
{
public:

    Transform();

    // Setters
    void SetPosition(const Vector3& position);
    void SetRotation(const Vector3& rotation);
    void SetScale(const Vector3& scale);

    void Translate(float x, float y, float z);
    void Rotate(float x, float y, float z);
    void Scale(float x, float y, float z);

    void SetParent(Transform* parent);

    // Getters
    const Vector3& GetPosition() const;
    const Vector3& GetRotation() const;
    const Vector3& GetScale() const;

    Transform* GetParent() const;
    const std::vector<Transform*>& GetChildren() const;
    std::size_t GetChildCount() const;

    // Matrices
    Matrix4 GetLocalMatrix() const;
    Matrix4 GetWorldMatrix() const;

private:

    Transform* m_Parent = nullptr;

    std::vector<Transform*> m_Children;

    Vector3 m_Position;
    Vector3 m_Rotation;
    Vector3 m_Scale = { 1.0f, 1.0f, 1.0f };
};

} // namespace AtlasEngine