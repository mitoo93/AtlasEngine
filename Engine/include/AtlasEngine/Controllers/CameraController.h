#pragma once

#include "AtlasEngine/Math/Vector2.h"

namespace AtlasEngine
{

class Camera;

class CameraController
{
public:

    void Update(
        Camera& camera,
        float deltaTime);

    void SetMoveSpeed(float speed);
    void SetRotationSpeed(float speed);
    void SetMouseSensitivity(float sensitivity);

private:

    float m_MoveSpeed = 2.0f;
    float m_RotationSpeed = 90.0f;
    
    float m_MouseSensitivity = 0.1f;
    bool m_FirstMouseUpdate = true;

    Vector2 m_LastMousePosition;
};

}