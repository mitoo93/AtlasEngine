#include "AtlasEngine/Editor/EditorLayer.h"
#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Math/Transform.h"
#include "AtlasEngine/Core/Application.h"

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

    ImGui::End();
    
    //------------------------------------
    // Scene Hierarchy
    //------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(10,50),
        ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowSize(
        ImVec2(250, 500),
        ImGuiCond_FirstUseEver);

    
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
        ImVec2(270,50),
        ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowSize(
        ImVec2(300, 500),
        ImGuiCond_FirstUseEver);

    
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
        ImVec2(580,50),
        ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowSize(
        ImVec2(220, 120),
        ImGuiCond_FirstUseEver);

    
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