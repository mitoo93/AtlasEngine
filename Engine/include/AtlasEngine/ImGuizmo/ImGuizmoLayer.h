#pragma once

#include "AtlasEngine/Editor/EditorCamera.h"
#include "AtlasEngine/Editor/GizmoMode.h"
#include "AtlasEngine/Editor/GizmoSpace.h"

#include <imgui.h>


namespace AtlasEngine
{

class Entity;


class ImGuizmoLayer
{
public:

    void OnImGuiRender(
        Entity* selectedEntity,
        const EditorCamera& camera,
        GizmoMode gizmoMode,
        GizmoSpace gizmoSpace,
        const ImVec2& viewportPosition,
        const ImVec2& viewportSize);

    bool IsUsing() const;

    bool IsOver() const;


private:

    void Manipulate(
        Entity* selectedEntity,
        const EditorCamera& camera,
        GizmoMode gizmoMode,
        GizmoSpace gizmoSpace);


    bool m_IsUsing =
        false;


    bool m_IsOver =
        false;
};

}