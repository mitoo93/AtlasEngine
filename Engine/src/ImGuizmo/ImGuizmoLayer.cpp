#include "AtlasEngine/ImGuizmo/ImGuizmoLayer.h"

#include "AtlasEngine/Scene/Entity.h"
#include "AtlasEngine/Math/Transform.h"
#include "AtlasEngine/Math/Matrix4.h"

#include <ImGuizmo.h>


namespace AtlasEngine
{

namespace
{

ImGuizmo::OPERATION ToImGuizmoOperation(
    GizmoMode gizmoMode)
{
    switch (gizmoMode)
    {
        case GizmoMode::Translate:
            return ImGuizmo::TRANSLATE;

        case GizmoMode::Rotate:
            return ImGuizmo::ROTATE;

        case GizmoMode::Scale:
            return ImGuizmo::SCALE;
    }

    return ImGuizmo::TRANSLATE;
}


ImGuizmo::MODE ToImGuizmoMode(
    GizmoSpace gizmoSpace)
{
    return
        gizmoSpace == GizmoSpace::World
            ? ImGuizmo::WORLD
            : ImGuizmo::LOCAL;
}

}


void ImGuizmoLayer::OnImGuiRender(
    Entity* selectedEntity,
    const EditorCamera& camera,
    GizmoMode gizmoMode,
    GizmoSpace gizmoSpace,
    const ImVec2& viewportPosition,
    const ImVec2& viewportSize)
{
    m_WasUsing =
        m_IsUsing;

    m_IsUsing =
        false;

    m_IsOver =
        false;


    if (!selectedEntity)
        return;


    if (viewportSize.x <= 0.0f ||
        viewportSize.y <= 0.0f)
    {
        return;
    }


    ImGuizmo::BeginFrame();

    ImGuizmo::SetDrawlist();

    ImGuizmo::SetOrthographic(false);


    ImGuizmo::SetRect(
        viewportPosition.x,
        viewportPosition.y,
        viewportSize.x,
        viewportSize.y);


    Manipulate(
        selectedEntity,
        camera,
        gizmoMode,
        gizmoSpace);


    m_IsOver =
        ImGuizmo::IsOver();
}


void ImGuizmoLayer::OnMiniGizmoRender(
    Entity* selectedEntity,
    const EditorCamera& camera,
    GizmoMode gizmoMode,
    GizmoSpace gizmoSpace,
    const ImVec2& position,
    const ImVec2& size)
{
    // Se implementará posteriormente.
}


void ImGuizmoLayer::Manipulate(
    Entity* selectedEntity,
    const EditorCamera& camera,
    GizmoMode gizmoMode,
    GizmoSpace gizmoSpace)
{
    Transform& transform =
        selectedEntity->GetTransform();


    Matrix4 worldMatrix =
        transform.GetWorldMatrix();


    Matrix4 view =
        camera.GetViewMatrix();


    Matrix4 projection =
        camera.GetProjectionMatrix();


    ImGuizmo::OPERATION operation =
        ToImGuizmoOperation(
            gizmoMode);


    ImGuizmo::MODE mode =
        ToImGuizmoMode(
            gizmoSpace);


    ImGuizmo::Manipulate(
        view.Data(),
        projection.Data(),
        operation,
        mode,
        worldMatrix.Data());


    if (!ImGuizmo::IsUsing())
        return;


    Matrix4 localMatrix =
        worldMatrix;


    Transform* parent =
        transform.GetParent();


    if (parent)
    {
        Matrix4 parentWorld =
            parent->GetWorldMatrix();


        localMatrix =
            parentWorld.Inverse() *
            worldMatrix;
    }


    float translation[3] {};

    float rotation[3] {};

    float scale[3] {};


    ImGuizmo::DecomposeMatrixToComponents(
        localMatrix.Data(),
        translation,
        rotation,
        scale);


    transform.SetPosition(
    {
        translation[0],
        translation[1],
        translation[2]
    });


    transform.SetRotation(
    {
        rotation[0],
        rotation[1],
        rotation[2]
    });


    transform.SetScale(
    {
        scale[0],
        scale[1],
        scale[2]
    });


    m_IsUsing =
        true;
}


bool ImGuizmoLayer::IsUsing() const
{
    return m_IsUsing;
}


bool ImGuizmoLayer::WasUsing() const
{
    return m_WasUsing;
}


bool ImGuizmoLayer::IsOver() const
{
    return m_IsOver;
}


}