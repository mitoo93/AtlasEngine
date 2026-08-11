#include "AtlasEngine/Editor/EditorLayer.h"
#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Math/Transform.h"
#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/ImGuizmo/ImGuizmoLayer.h"

#include <imgui.h>
#include <iostream>

namespace AtlasEngine
{

void EditorLayer::OnAttach()
{

    m_State = EditorState::Edit;
    /* Application::Get()
        .GetWindow()
        .SetCursorEnabled(true); */
    
    std::cout << "EditorLayer Attach\n";
}

void EditorLayer::OnUpdate(float)
{
}

void EditorLayer::OnDetach()
{
    std::cout << "EditorLayer Detach\n";
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
    
    //------------------------------------
    // Layout
    //------------------------------------

    const float toolbarHeight = 40.0f;

    const float hierarchyWidth = 250.0f;
    const float inspectorWidth = 320.0f;
    const float statsHeight = 120.0f;

    ImVec2 display = ImGui::GetIO().DisplaySize;

    float viewportX = hierarchyWidth;
    float viewportY = toolbarHeight;

    float viewportWidth =
        display.x -
        hierarchyWidth -
        inspectorWidth;

    float viewportHeight =
        display.y -
        toolbarHeight -
        statsHeight;

    //------------------------------------
    // Toolbar
    //------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(0, 0),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(
            ImGui::GetIO().DisplaySize.x,
            40),
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
            std::cout << "PLAY\n";

            m_State = EditorState::Play;

            /* Application::Get()
                .GetWindow()
                .SetCursorEnabled(false); */
        }
    }
    else
    {
        if (ImGui::Button("Stop"))
        {
            std::cout << "STOP\n";

            m_State = EditorState::Edit;

            /* Application::Get()
                .GetWindow()
                .SetCursorEnabled(true); */
        }
    }

    ImGui::SameLine();
    ImGui::Text("| Estado:");

    ImGui::SameLine();

    if (m_State == EditorState::Play)
    {
        ImGui::TextColored(
            ImVec4(0.2f, 1.0f, 0.2f, 1.0f),
            "PLAY");
    }
    else
    {
        ImGui::TextColored(
            ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
            "EDIT");
    }

    ImGui::End();

    //------------------------------------
    // Viewport
    //------------------------------------

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

    ImVec2 viewportPos =
        ImGui::GetWindowPos();

    viewportPos.x += ImGui::GetWindowContentRegionMin().x;
    viewportPos.y += ImGui::GetWindowContentRegionMin().y;

    m_ViewportPosition = viewportPos;

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    
    m_ViewportSize = ImGui::GetContentRegionAvail();

    if (m_ViewportSize.x > 0 &&
        m_ViewportSize.y > 0)
    {
        if (m_ViewportSize.x != m_LastViewportSize.x ||
            m_ViewportSize.y != m_LastViewportSize.y)
        {
            Renderer::ResizeFramebuffer(
                (uint32_t)m_ViewportSize.x,
                (uint32_t)m_ViewportSize.y);

            m_LastViewportSize = m_ViewportSize;
        }
    }

    ImGui::Text(
        "Focused: %s",
        m_ViewportFocused ? "Yes" : "No");

    ImGui::Text(
        "Hovered: %s",
        m_ViewportHovered ? "Yes" : "No");

    ImGui::Image(
        (ImTextureID)(intptr_t)
            Renderer::GetFramebuffer().GetColorAttachment(),
        m_ViewportSize,
        ImVec2(0, 1),
        ImVec2(1, 0));

    if (m_SelectedEntity)
        ImGui::Text("Entidad seleccionada");
    else
        ImGui::Text("NO HAY ENTIDAD");
    
    m_GizmoLayer.OnImGuiRender(
        m_SelectedEntity,
        m_Context,
        m_ViewportPosition,
        m_ViewportSize);

    ImGui::End();
        
    //------------------------------------
    // Scene Hierarchy
    //------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(0, toolbarHeight),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(
            hierarchyWidth,
            display.y - toolbarHeight),
        ImGuiCond_Always);

    
    ImGui::Begin("Scene Hierarchy");

    if (m_Context)
    {
        for (const auto& entity : m_Context->GetEntities())
            {

                bool selected = (m_SelectedEntity == entity.get());

                if (ImGui::Selectable(entity->GetName().c_str(), selected))
                    m_SelectedEntity = entity.get();
            }
    }

    ImGui::End();

    //------------------------------------
    // Inspector
    //------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(
            display.x - inspectorWidth,
            toolbarHeight),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(
            inspectorWidth,
            display.y - toolbarHeight),
        ImGuiCond_Always);

    
    ImGui::Begin("Inspector");

    if (m_SelectedEntity)
    {
        ImGui::Text(
            "Nombre: %s",
            m_SelectedEntity->GetName().c_str());

        ImGui::Separator();

        if (ImGui::CollapsingHeader(
                "Transform",
                ImGuiTreeNodeFlags_DefaultOpen))
        {
            Transform& transform =
                m_SelectedEntity->GetTransform();

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
                transform.SetPosition(position);
            }

            if (ImGui::DragFloat3(
                    "Rotation",
                    &rotation.x,
                    0.5f))
            {
                transform.SetRotation(rotation);
            }

            if (ImGui::DragFloat3(
                    "Scale",
                    &scale.x,
                    0.05f))
            {
                transform.SetScale(scale);
            }
        }
    }
    else
    {
        ImGui::Text("Ninguna entidad seleccionada");
    }

    ImGui::End();

    //------------------------------------
    // Statistics
    //------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(
            hierarchyWidth,
            display.y - statsHeight),
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
        1000.0f / ImGui::GetIO().Framerate);

    ImGui::End();
}

void EditorLayer::SetContext(Scene* scene)
{
    m_Context = scene;
}

}