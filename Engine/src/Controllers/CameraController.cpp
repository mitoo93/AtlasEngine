#include <iostream>
#include <cstring>

#include "AtlasEngine/Controllers/CameraController.h"

#include "AtlasEngine/Core/Input.h"
#include "AtlasEngine/Core/Key.h"
#include "AtlasEngine/Renderer/Camera.h"

namespace AtlasEngine
{

void CameraController::Update(
    Camera& camera,
    float deltaTime)
{

    float movement =
        m_MoveSpeed * deltaTime;

    Vector3 movementDirection;

    if (Input::IsKeyDown(Key::W))
    {
        movementDirection +=
            camera.GetForward();
    }

    if (Input::IsKeyDown(Key::S))
    {
        movementDirection -=
            camera.GetForward();
    }

    if (Input::IsKeyDown(Key::D))
    {
        movementDirection +=
            camera.GetRight();
    }

    if (Input::IsKeyDown(Key::A))
    {
        movementDirection -=
            camera.GetRight();
    }

    if (Input::IsKeyDown(Key::Q))
    {
        movementDirection +=
            camera.GetUp();
    }

    if (Input::IsKeyDown(Key::E))
    {
        movementDirection -=
            camera.GetUp();
    }

    if (movementDirection.Length() > 0.0f)
    {
        movementDirection =
            movementDirection.Normalized();

        camera.Translate(
            movementDirection *
            movement);
    }

    Vector2 currentMouse =
        Input::GetMousePosition();

    if (m_FirstMouseUpdate)
    {
        m_LastMousePosition = currentMouse;
        m_FirstMouseUpdate = false;
        return;
    }

    Vector2 mouseDelta =
        currentMouse -
        m_LastMousePosition;

    m_LastMousePosition =
        currentMouse;

    Vector3 rotation =
        camera.GetRotation();

    rotation.y +=
        mouseDelta.x *
        m_MouseSensitivity;

    rotation.x -=
        mouseDelta.y *
        m_MouseSensitivity;

    if (rotation.x > 89.0f)
    {
        rotation.x = 89.0f;
    }

    if (rotation.x < -89.0f)
    {
        rotation.x = -89.0f;
    }

    camera.SetRotation(rotation);

}

void CameraController::SetMoveSpeed(float speed)
{
    m_MoveSpeed = speed;
}

void CameraController::SetRotationSpeed(float speed)
{
    m_RotationSpeed = speed;
}

void CameraController::SetMouseSensitivity(float sensitivity)
{
    m_MouseSensitivity = sensitivity;
}

}