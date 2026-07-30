#include <iostream>

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

    float rotation =
        m_RotationSpeed * deltaTime;

    if (Input::IsKeyPressed(Key::W))
    {
        camera.Translate(
            camera.GetForward() *
            movement);
    }

    if (Input::IsKeyPressed(Key::S))
    {
        camera.Translate(
            camera.GetForward() *
            -movement);
    }

    if (Input::IsKeyPressed(Key::A))
    {
        camera.Translate(
            camera.GetRight() *
            -movement);
    }

    if (Input::IsKeyPressed(Key::D))
    {
        camera.Translate(
            camera.GetRight() *
            movement);
    }

    /* if (Input::IsKeyPressed(Key::Left))
    {
        camera.Rotate(
            0.0f,
            -rotation,
            0.0f);
    }

    if (Input::IsKeyPressed(Key::Right))
    {
        camera.Rotate(
            0.0f,
            rotation,
            0.0f);
    }

    if (Input::IsKeyPressed(Key::Up))
    {
        camera.Rotate(
            -rotation,
            0.0f,
            0.0f);
    }

    if (Input::IsKeyPressed(Key::Down))
    {
        camera.Rotate(
            rotation,
            0.0f,
            0.0f);
    } */

    if (Input::IsKeyPressed(Key::Q))
    {
        camera.Translate(
            camera.GetUp() *
            movement);
    }

    if (Input::IsKeyPressed(Key::E))
    {
        camera.Translate(
            camera.GetUp() *
            -movement);
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

    camera.Rotate(
        -mouseDelta.y * m_MouseSensitivity,
        mouseDelta.x * m_MouseSensitivity,
        0.0f);

    if (mouseDelta.x != 0.0f ||
        mouseDelta.y != 0.0f)
    /* {
        std::cout
            << "Mouse Delta: "
            << mouseDelta.x
            << ", "
            << mouseDelta.y
            << '\n';
    } */
    camera.Rotate(
        -mouseDelta.y * m_MouseSensitivity,
        mouseDelta.x * m_MouseSensitivity,
        0.0f);
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