#include <cmath>

#include "AtlasEngine/Editor/EditorCamera.h"

#include "AtlasEngine/Core/Input.h"
#include "AtlasEngine/Core/Key.h"
#include "AtlasEngine/Core/MouseButton.h"

#include "AtlasEngine/Math/Math.h"

#include <iostream>


namespace AtlasEngine
{


EditorCamera::EditorCamera()
    :
    m_Position(
    {
        0.0f,
        0.0f,
        5.0f
    }),
    m_Rotation(
    {
        0.0f,
        0.0f,
        0.0f
    })
{
}


void EditorCamera::Update(
    float deltaTime,
    bool allowInput,
    bool allowMovement,
    bool allowRotation)
{
    if (!allowInput)
    {
        ResetMouseState();
        return;
    }


    // ----------------------------------------------------
    // La cámara de editor SOLO funciona con RMB.
    // ----------------------------------------------------

    if (!Input::IsMouseButtonDown(
            MouseButton::Right))
    {
        ResetMouseState();
        return;
    }


    // ----------------------------------------------------
    // Mouse
    // ----------------------------------------------------

    Vector2 currentMouse =
        Input::GetMousePosition();


    if (m_FirstMouseUpdate)
    {
        m_LastMousePosition =
            currentMouse;

        m_FirstMouseUpdate =
            false;

        return;
    }


    Vector2 mouseDelta =
        currentMouse -
        m_LastMousePosition;


    m_LastMousePosition =
        currentMouse;


    // ----------------------------------------------------
    // ROTATION
    // ----------------------------------------------------

    if (allowRotation)
    {
        m_Rotation.y +=
            mouseDelta.x *
            m_MouseSensitivity;


        m_Rotation.x -=
            mouseDelta.y *
            m_MouseSensitivity;


        if (m_Rotation.x > 89.0f)
        {
            m_Rotation.x = 89.0f;
        }


        if (m_Rotation.x < -89.0f)
        {
            m_Rotation.x = -89.0f;
        }
    }

    std::cout
        << "W=" << Input::IsKeyDown(Key::W)
        << " S=" << Input::IsKeyDown(Key::S)
        << " A=" << Input::IsKeyDown(Key::A)
        << " D=" << Input::IsKeyDown(Key::D)
        << " Q=" << Input::IsKeyDown(Key::Q)
        << " E=" << Input::IsKeyDown(Key::E)
        << "\n";


    // ----------------------------------------------------
    // MOVEMENT
    // ----------------------------------------------------

    std::cout
        << "CAMERA INPUT - allowInput: "
        << allowInput
        << " allowMovement: "
        << allowMovement
        << " RMB: "
        << Input::IsMouseButtonDown(MouseButton::Right)
        << "\n";
    
    if (allowMovement)
    {
         std::cout
            << "W=" << Input::IsKeyDown(Key::W)
            << " S=" << Input::IsKeyDown(Key::S)
            << " A=" << Input::IsKeyDown(Key::A)
            << " D=" << Input::IsKeyDown(Key::D)
            << " Q=" << Input::IsKeyDown(Key::Q)
            << " E=" << Input::IsKeyDown(Key::E)
            << " dt=" << deltaTime
            << "\n";

        const float movement =
            m_MoveSpeed * deltaTime;


        Vector3 movementDirection;

        std::cout
            << "MOVEMENT DIR: "
            << movementDirection.x << " "
            << movementDirection.y << " "
            << movementDirection.z
            << "\n";


        if (Input::IsKeyDown(Key::W))
        {
            movementDirection +=
                GetForward();
        }


        if (Input::IsKeyDown(Key::S))
        {
            movementDirection -=
                GetForward();
        }


        if (Input::IsKeyDown(Key::D))
        {
            movementDirection +=
                GetRight();
        }


        if (Input::IsKeyDown(Key::A))
        {
            movementDirection -=
                GetRight();
        }


        if (Input::IsKeyDown(Key::Q))
        {
            movementDirection +=
                GetUp();
        }


        if (Input::IsKeyDown(Key::E))
        {
            movementDirection -=
                GetUp();
        }


        if (movementDirection.Length() > 0.0f)
        {
            movementDirection =
                movementDirection.Normalized();


            m_Position +=
                movementDirection *
                m_MoveSpeed *
                deltaTime;

            std::cout
                << "MOVEMENT DIR: "
                << movementDirection.x << " "
                << movementDirection.y << " "
                << movementDirection.z
                << "\n";
        }
    }
}


void EditorCamera::SetViewportSize(
    float width,
    float height)
{
    if (width <= 0.0f ||
        height <= 0.0f)
    {
        return;
    }


    m_AspectRatio =
        width / height;
}


Matrix4 EditorCamera::GetViewMatrix() const
{
    return
        Matrix4::RotateX(-m_Rotation.x)
        *
        Matrix4::RotateY(m_Rotation.y)
        *
        Matrix4::RotateZ(-m_Rotation.z)
        *
        Matrix4::Translate(
            -m_Position.x,
            -m_Position.y,
            -m_Position.z);
}


Matrix4 EditorCamera::GetProjectionMatrix() const
{
    return Matrix4::Perspective(
        m_Fov,
        m_AspectRatio,
        m_NearPlane,
        m_FarPlane);
}


Vector3 EditorCamera::GetForward() const
{
    const float pitch =
        Math::ToRadians(m_Rotation.x);

    const float yaw =
        Math::ToRadians(m_Rotation.y);


    Vector3 forward;


    forward.x =
        std::cos(pitch) *
        std::sin(yaw);


    forward.y =
        std::sin(pitch);


    forward.z =
        -std::cos(pitch) *
        std::cos(yaw);


    return forward.Normalized();
}


Vector3 EditorCamera::GetRight() const
{
    const Vector3 worldUp
    {
        0.0f,
        1.0f,
        0.0f
    };


    return
        Vector3::Cross(
            GetForward(),
            worldUp)
        .Normalized();
}


Vector3 EditorCamera::GetUp() const
{
    return
        Vector3::Cross(
            GetRight(),
            GetForward())
        .Normalized();
}


void EditorCamera::Focus(
    const Vector3& target,
    float distance)
{
    if (distance <= 0.0f)
    {
        distance = 1.0f;
    }


    Vector3 direction =
        m_Position - target;


    if (direction.Length() <= 0.0001f)
    {
        direction =
            Vector3
            {
                0.0f,
                0.0f,
                1.0f
            };
    }


    direction =
        direction.Normalized();


    m_Position =
        target +
        direction * distance;


    Vector3 lookDirection =
        target - m_Position;


    lookDirection =
        lookDirection.Normalized();


    const float yaw =
        std::atan2(
            lookDirection.x,
            -lookDirection.z);


    const float pitch =
        std::asin(lookDirection.y);


    m_Rotation.y =
        Math::ToDegrees(yaw);


    m_Rotation.x =
        Math::ToDegrees(pitch);

    Vector3 forward = GetForward();

    std::cout
        << "FOCUS rotation: "
        << m_Rotation.x << ", "
        << m_Rotation.y << ", "
        << m_Rotation.z
        << "\n";

    std::cout
        << "FOCUS forward: "
        << forward.x << ", "
        << forward.y << ", "
        << forward.z
        << "\n";

    std::cout
        << "FOCUS target: "
        << target.x << ", "
        << target.y << ", "
        << target.z
        << "\n";

    Vector3 toTarget =
        (target - m_Position).Normalized();

    std::cout
        << "FOCUS toTarget: "
        << toTarget.x << ", "
        << toTarget.y << ", "
        << toTarget.z
        << "\n";


    ResetMouseState();
}


void EditorCamera::ResetMouseState()
{
    m_FirstMouseUpdate = true;

    m_LastMousePosition =
        Input::GetMousePosition();
}


} // namespace AtlasEngine