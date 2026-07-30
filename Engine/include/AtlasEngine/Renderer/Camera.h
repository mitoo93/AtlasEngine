#pragma once

#include "AtlasEngine/Math/Vector3.h"
#include "AtlasEngine/Math/Matrix4.h"

namespace AtlasEngine
{

class Camera
{
public:

    void SetPosition(const Vector3& position);
    void SetRotation(const Vector3& rotation);

    void Translate(float x, float y, float z);
    void Translate(const Vector3& translation);
    void Rotate(float x, float y, float z);
    void Rotate(const Vector3& rotation);
    
    void SetAspectRatio(float aspectRatio);

    const Vector3& GetPosition() const;
    const Vector3& GetRotation() const;

    Matrix4 GetViewMatrix() const;
    Matrix4 GetProjectionMatrix() const;

    Vector3 GetForward() const;
    Vector3 GetRight() const;
    Vector3 GetUp() const;

private:

    Vector3 m_Position;
    Vector3 m_Rotation;

    float m_Fov = 45.0f;
    float m_AspectRatio = 16.0f / 9.0f;

    float m_NearPlane = 0.1f;
    float m_FarPlane = 100.0f;
};

}