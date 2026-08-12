#include "AtlasEngine/Editor/EditorLayer.h"

#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Math/Transform.h"
#include "AtlasEngine/Core/Input.h"
#include "AtlasEngine/Core/Key.h"
#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Renderer/Renderer.h"

#include <imgui.h>
#include <iostream>

namespace AtlasEngine
{

void EditorLayer::OnAttach()
{
    m_State =
        EditorState::Edit;

    std::cout
        << "EditorLayer Attach\n";

    std::cout
        << "EditorCamera Pos: "
        << m_EditorCamera.GetPosition().x
        << " "
        << m_EditorCamera.GetPosition().y
        << " "
        << m_EditorCamera.GetPosition().z
        << "\n";
}

void EditorLayer::OnUpdate(float deltaTime)
{
    m_DeltaTime = deltaTime;
    
    if (!IsEditing())
        return;


    // ----------------------------------------------------
    // GIZMO MODE
    // ----------------------------------------------------

    if (Input::IsKeyPressed(Key::T))
    {
        m_GizmoMode =
            GizmoMode::Translate;
    }


    if (Input::IsKeyPressed(Key::Y))
    {
        m_GizmoMode =
            GizmoMode::Rotate;
    }


    if (Input::IsKeyPressed(Key::U))
    {
        m_GizmoMode =
            GizmoMode::Scale;
    }


    // ----------------------------------------------------
    // GIZMO SPACE
    // ----------------------------------------------------

    if (Input::IsKeyPressed(Key::X))
    {
        m_GizmoSpace =
            m_GizmoSpace == GizmoSpace::Local
                ? GizmoSpace::World
                : GizmoSpace::Local;
    }


    // ----------------------------------------------------
    // FOCUS
    // ----------------------------------------------------

    if (m_SelectedEntity &&
        Input::IsKeyPressed(Key::G) &&
        !m_GizmoLayer.IsUsing() &&
        !m_GizmoLayer.IsOver())
    {
        Matrix4 world =
            m_SelectedEntity
                ->GetTransform()
                .GetWorldMatrix();


        Vector3 target
        {
            world.GetTranslationX(),
            world.GetTranslationY(),
            world.GetTranslationZ()
        };

        std::cout
            << "FOCUS target: "
            << target.x << ", "
            << target.y << ", "
            << target.z
            << "\n";

        std::cout
            << "CAMERA before: "
            << m_EditorCamera.GetPosition().x << ", "
            << m_EditorCamera.GetPosition().y << ", "
            << m_EditorCamera.GetPosition().z
            << "\n";

        m_EditorCamera.Focus(
            target,
            5.0f);

        std::cout
            << "CAMERA after: "
            << m_EditorCamera.GetPosition().x << ", "
            << m_EditorCamera.GetPosition().y << ", "
            << m_EditorCamera.GetPosition().z
            << "\n";
    }


    // ----------------------------------------------------
    // CAMERA ROTATION TOGGLE
    // ----------------------------------------------------

    if (Input::IsKeyPressed(Key::F))
    {
        m_CameraRotationEnabled =
            !m_CameraRotationEnabled;


        m_EditorCamera.ResetMouseState();
    }

    // ----------------------------------------------------
    // EDITOR CAMERA
    // ----------------------------------------------------

    const bool allowCamera =
        m_ViewportHovered &&
        !m_GizmoLayer.IsOver() &&
        !m_GizmoLayer.IsUsing();

    m_EditorCamera.Update(
        m_DeltaTime,
        allowCamera,
        true,
        m_CameraRotationEnabled);

}

void EditorLayer::OnDetach()
{
    std::cout
        << "EditorLayer Detach\n";
}

void EditorLayer::OnImGuiRender()
{
    if (!m_SelectedEntity &&
        m_Context &&
        !m_Context->GetEntities().empty())
    {
        m_SelectedEntity =
            m_Context->GetEntities().front().get();
    }


    // ----------------------------------------------------
    // LAYOUT
    // ----------------------------------------------------

    const float toolbarHeight = 40.0f;
    const float hierarchyWidth = 250.0f;
    const float inspectorWidth = 320.0f;
    const float statsHeight = 120.0f;

    ImVec2 display =
        ImGui::GetIO().DisplaySize;

    float viewportX =
        hierarchyWidth;

    float viewportY =
        toolbarHeight;

    float viewportWidth =
        display.x -
        hierarchyWidth -
        inspectorWidth;

    float viewportHeight =
        display.y -
        toolbarHeight -
        statsHeight;

    // ----------------------------------------------------
    // TOOLBAR
    // ----------------------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(0, 0),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(display.x, toolbarHeight),
        ImGuiCond_Always);

    ImGui::Begin(
        "Toolbar",
        nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize);

    if (m_State == EditorState::Edit)
    {
        if (ImGui::Button("Play"))
        {
            m_State =
                EditorState::Play;
        }
    }
    else
    {
        if (ImGui::Button("Stop"))
        {
            m_State =
                EditorState::Edit;
        }
    }

    ImGui::SameLine();
    ImGui::Text("| Estado:");

    ImGui::SameLine();

    if (m_State == EditorState::Play)
    {
        ImGui::TextColored(
            ImVec4(
                0.2f,
                1.0f,
                0.2f,
                1.0f),
            "PLAY");
    }
    else
    {
        ImGui::TextColored(
            ImVec4(
                1.0f,
                0.8f,
                0.2f,
                1.0f),
            "EDIT");
    }

    ImGui::SameLine();
    ImGui::Text("| Gizmo:");

    ImGui::SameLine();

    const char* gizmoModeText = "";

    switch (m_GizmoMode)
    {
        case GizmoMode::Translate:
            gizmoModeText = "Translate";
            break;

        case GizmoMode::Rotate:
            gizmoModeText = "Rotate";
            break;

        case GizmoMode::Scale:
            gizmoModeText = "Scale";
            break;
    }

    ImGui::Text("%s", gizmoModeText);

    ImGui::SameLine();
    ImGui::Text("| Space:");

    ImGui::SameLine();

    ImGui::Text(
        "%s",
        m_GizmoSpace == GizmoSpace::Local
            ? "Local"
            : "World");

    ImGui::SameLine();
    ImGui::Text("| Camera:");

    ImGui::SameLine();

    ImGui::Text(
        "%s",
        m_CameraRotationEnabled
            ? "Rotation ON"
            : "Rotation OFF");

    ImGui::End();

    // ----------------------------------------------------
    // VIEWPORT
    // ----------------------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(
            viewportX,
            viewportY),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(
            viewportWidth,
            viewportHeight),
        ImGuiCond_Always);

    ImGui::Begin("Viewport");

    m_ViewportFocused =
        ImGui::IsWindowFocused(
            ImGuiFocusedFlags_RootAndChildWindows);

    m_ViewportHovered =
        ImGui::IsWindowHovered(
            ImGuiHoveredFlags_RootAndChildWindows);

    ImVec2 imagePosition =
        ImGui::GetCursorScreenPos();

    ImVec2 imageSize =
        ImGui::GetContentRegionAvail();

    m_ViewportPosition =
        imagePosition;

    m_ViewportSize =
        imageSize;

    m_EditorCamera.SetViewportSize(
        m_ViewportSize.x,
        m_ViewportSize.y);

    if (m_ViewportSize.x > 0.0f &&
        m_ViewportSize.y > 0.0f)
    {
        if (m_ViewportSize.x != m_LastViewportSize.x ||
            m_ViewportSize.y != m_LastViewportSize.y)
        {
            Renderer::ResizeFramebuffer(
                static_cast<uint32_t>(m_ViewportSize.x),
                static_cast<uint32_t>(m_ViewportSize.y));

            m_LastViewportSize =
                m_ViewportSize;
        }
    }

    ImGui::Image(
        (ImTextureID)(intptr_t)
            Renderer::GetFramebuffer()
                .GetColorAttachment(),
        m_ViewportSize,
        ImVec2(0, 1),
        ImVec2(1, 0));

    m_GizmoLayer.OnImGuiRender(
        m_SelectedEntity,
        m_EditorCamera,
        m_GizmoMode,
        m_GizmoSpace,
        m_ViewportPosition,
        m_ViewportSize);

    ImGui::End();

    // ----------------------------------------------------
    // MINI GIZMO
    // ----------------------------------------------------

    const float miniGizmoSize =
        120.0f;

    const float miniGizmoMargin =
        15.0f;

    ImVec2 miniGizmoPosition
    {
        m_ViewportPosition.x +
            m_ViewportSize.x -
            miniGizmoSize -
            miniGizmoMargin,

        m_ViewportPosition.y +
            miniGizmoMargin
    };

    m_GizmoLayer.OnMiniGizmoRender(
        m_SelectedEntity,
        m_EditorCamera,
        m_GizmoMode,
        m_GizmoSpace,
        miniGizmoPosition,
        ImVec2(
            miniGizmoSize,
            miniGizmoSize));

    // ----------------------------------------------------
    // SCENE HIERARCHY
    // ----------------------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(
            0,
            toolbarHeight),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(
            hierarchyWidth,
            display.y -
                toolbarHeight),
        ImGuiCond_Always);

    ImGui::Begin("Scene Hierarchy");

    if (m_Context)
    {
        for (const auto& entity :
             m_Context->GetEntities())
        {
            bool selected =
                m_SelectedEntity ==
                entity.get();

            if (ImGui::Selectable(
                    entity->GetName().c_str(),
                    selected))
            {
                m_SelectedEntity =
                    entity.get();
            }
        }
    }

    ImGui::End();

    // ----------------------------------------------------
    // INSPECTOR
    // ----------------------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(
            display.x -
                inspectorWidth,
            toolbarHeight),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(
            inspectorWidth,
            display.y -
                toolbarHeight),
        ImGuiCond_Always);

    ImGui::Begin("Inspector");

    if (m_SelectedEntity)
    {
        ImGui::Text(
            "Nombre: %s",
            m_SelectedEntity
                ->GetName()
                .c_str());

        ImGui::Separator();

        if (ImGui::CollapsingHeader(
                "Transform",
                ImGuiTreeNodeFlags_DefaultOpen))
        {
            Transform& transform =
                m_SelectedEntity
                    ->GetTransform();

            Vector3 position =
                transform.GetPosition();

            Vector3 rotation =
                transform.GetRotation();

            Vector3 scale =
                transform.GetScale();

            if (ImGui::DragFloat3(
                    "Position",
                    &position.x,
                    0.05f))
            {
                transform.SetPosition(
                    position);
            }

            if (ImGui::DragFloat3(
                    "Rotation",
                    &rotation.x,
                    0.5f))
            {
                transform.SetRotation(
                    rotation);
            }

            if (ImGui::DragFloat3(
                    "Scale",
                    &scale.x,
                    0.05f))
            {
                transform.SetScale(
                    scale);
            }
        }
    }
    else
    {
        ImGui::Text(
            "Ninguna entidad seleccionada");
    }

    ImGui::End();

    // ----------------------------------------------------
    // STATISTICS
    // ----------------------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(
            hierarchyWidth,
            display.y -
                statsHeight),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(
            viewportWidth,
            statsHeight),
        ImGuiCond_Always);

    ImGui::Begin("Statistics");

    ImGui::Text(
        "FPS: %.1f",
        ImGui::GetIO().Framerate);

    ImGui::Text(
        "Frame Time: %.3f ms",
        1000.0f /
            ImGui::GetIO().Framerate);

    ImGui::End();
}

void EditorLayer::SetContext(Scene* scene)
{
    m_Context = scene;
}

}