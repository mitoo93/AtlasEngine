#pragma once

#include "AtlasEngine/Math/Vector2.h"
#include "AtlasEngine/Math/Vector3.h"
#include "AtlasEngine/Math/Matrix4.h"
#include "AtlasEngine/Math/Ray.h"

namespace AtlasEngine
{

class EditorCamera
{
public:

    EditorCamera();

    void Update(
        float deltaTime,
        bool allowInput,
        bool allowRotation);

    void SetViewportSize(
        float width,
        float height);

    Matrix4 GetViewMatrix() const;

    Matrix4 GetProjectionMatrix() const;

    const Vector3& GetPosition() const
    {
        return m_Position;
    }


    Vector3 GetForward() const;
    Vector3 GetRight() const;
    Vector3 GetUp() const;

    void Focus(
        const Vector3& target,
        float distance = 5.0f);

    void ResetMouseState();

    Ray ScreenPointToRay(
        float mouseX,
        float mouseY,
        float viewportWidth,
        float viewportHeight) const;

private:

    Vector3 m_Position;
    Vector3 m_Rotation;

    Vector2 m_LastMousePosition;

    float m_Fov = 45.0f;
    float m_AspectRatio = 16.0f / 9.0f;
    float m_NearPlane = 0.1f;
    float m_FarPlane = 1000.0f;

    float m_MoveSpeed = 5.0f;
    float m_MouseSensitivity = 0.15f;

    bool m_FirstMouseUpdate = true;
};

}