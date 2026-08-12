#pragma once

#include "AtlasEngine/Core/Layer.h"
#include "AtlasEngine/ImGuizmo/ImGuizmoLayer.h"
#include "AtlasEngine/Editor/EditorCamera.h"
#include "AtlasEngine/Editor/GizmoMode.h"
#include "AtlasEngine/Editor/GizmoSpace.h"

#include <imgui.h>


namespace AtlasEngine
{

class Scene;
class Entity;


class EditorLayer : public Layer
{
public:

    EditorLayer()
        :
        Layer("EditorLayer")
    {
    }


    enum class EditorState
    {
        Edit = 0,
        Play
    };


    void OnAttach() override;

    void OnUpdate(
        float dt) override;

    void OnDetach() override;

    void OnImGuiRender() override;


    void SetContext(
        Scene* scene);


    EditorState GetState() const
    {
        return m_State;
    }


    bool IsPlaying() const
    {
        return m_State ==
            EditorState::Play;
    }


    bool IsEditing() const
    {
        return m_State ==
            EditorState::Edit;
    }


    const ImVec2& GetViewportSize() const
    {
        return m_ViewportSize;
    }


    bool IsViewportFocused() const
    {
        return m_ViewportFocused;
    }


    bool IsViewportHovered() const
    {
        return m_ViewportHovered;
    }


    const ImVec2& GetViewportPosition() const
    {
        return m_ViewportPosition;
    }


    const EditorCamera& GetEditorCamera() const
    {
        return m_EditorCamera;
    }


private:

    Scene* m_Context = nullptr;

    ImGuizmoLayer m_GizmoLayer;

    EditorCamera m_EditorCamera;

    Entity* m_SelectedEntity = nullptr;

    EditorState m_State =
        EditorState::Edit;


    ImVec2 m_ViewportSize =
    {
        0.0f,
        0.0f
    };


    ImVec2 m_LastViewportSize =
    {
        0.0f,
        0.0f
    };


    bool m_ViewportFocused =
        false;

    bool m_ViewportHovered =
        false;


    ImVec2 m_ViewportPosition =
    {
        0.0f,
        0.0f
    };


    GizmoMode m_GizmoMode =
        GizmoMode::Translate;


    GizmoSpace m_GizmoSpace =
        GizmoSpace::Local;


    bool m_CameraRotationEnabled =
        true;

    float m_DeltaTime = 0.0f;
};

}