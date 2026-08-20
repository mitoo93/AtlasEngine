#include "AtlasEngine/Editor/EditorLayer.h"

#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/MeshRendererComponent.h"
#include "AtlasEngine/Scene/ModelRendererComponent.h"
#include "AtlasEngine/Scene/MaterialComponent.h"
#include "AtlasEngine/Math/Transform.h"
#include "AtlasEngine/Core/Input.h"
#include "AtlasEngine/Core/Key.h"
#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Renderer/Model.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Material.h"

#include "AtlasEngine/Assets/AssetManager.h"

#include <imgui.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <filesystem>

namespace AtlasEngine
{

void EditorLayer::OnAttach()
{
    m_State =
        EditorState::Edit;

    std::cout
        << "EditorLayer Attach\n";

}

void EditorLayer::OnUpdate(float deltaTime)
{
    
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

        m_EditorCamera.Focus(
            target,
            5.0f);

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
        deltaTime,
        allowCamera,
        m_CameraRotationEnabled);

    // ----------------------------------------------------
    // PICKING
    // ----------------------------------------------------

    if (m_ViewportHovered &&
        Input::IsMouseButtonPressed(MouseButton::Left) &&
        !m_GizmoLayer.IsOver() &&
        !m_GizmoLayer.IsUsing())
    {
        Vector2 mousePosition =
            Input::GetMousePosition();

        float mouseX =
            mousePosition.x -
            m_ViewportPosition.x;

        float mouseY =
            mousePosition.y -
            m_ViewportPosition.y;

        if (mouseX >= 0.0f &&
            mouseX <= m_ViewportSize.x &&
            mouseY >= 0.0f &&
            mouseY <= m_ViewportSize.y)
        {
            Ray ray =
                m_EditorCamera.ScreenPointToRay(
                    mouseX,
                    mouseY,
                    m_ViewportSize.x,
                    m_ViewportSize.y);

            Entity* pickedEntity =
                PickEntity(ray);

            m_SelectedEntity =
                pickedEntity;
        }
    }

}

void EditorLayer::OnDetach()
{
    std::cout
        << "EditorLayer Detach\n";
}

void EditorLayer::OnImGuiRender()
{

    // ----------------------------------------------------
    // LAYOUT
    // ----------------------------------------------------

    const float toolbarHeight = 40.0f;
    const float hierarchyWidth = 250.0f;
    const float inspectorWidth = 320.0f;
    const float statsHeight = 120.0f;
    const float assetBrowserHeight = 220.0f;

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

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload =
                ImGui::AcceptDragDropPayload("ASSET_PATH"))
        {
            const char* assetPath =
                static_cast<const char*>(payload->Data);

            if (assetPath)
            {
                HandleAssetDrop(assetPath);
            }
        }

        ImGui::EndDragDropTarget();
    }

    m_GizmoLayer.OnImGuiRender(
        m_SelectedEntity,
        m_EditorCamera,
        m_GizmoMode,
        m_GizmoSpace,
        m_ViewportPosition,
        m_ViewportSize);

    ImGui::End();

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
            toolbarHeight -
            assetBrowserHeight),
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
    // ASSET BROWSER
    // ----------------------------------------------------

    ImGui::SetNextWindowPos(
        ImVec2(
            0.0f,
            toolbarHeight +
            (display.y - toolbarHeight) -
            assetBrowserHeight),
        ImGuiCond_Always);

    ImGui::SetNextWindowSize(
        ImVec2(
            hierarchyWidth,
            assetBrowserHeight),
        ImGuiCond_Always);

    m_AssetBrowser.OnImGuiRender();

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

bool EditorLayer::RayIntersectsAABB(
    const Ray& ray,
    const Vector3& boundsMin,
    const Vector3& boundsMax,
    float& distance) const
{
    float tMin = 0.0f;
    float tMax = 100000.0f;

    const float origin[3] =
    {
        ray.origin.x,
        ray.origin.y,
        ray.origin.z
    };

    const float direction[3] =
    {
        ray.direction.x,
        ray.direction.y,
        ray.direction.z
    };

    const float minBounds[3] =
    {
        boundsMin.x,
        boundsMin.y,
        boundsMin.z
    };

    const float maxBounds[3] =
    {
        boundsMax.x,
        boundsMax.y,
        boundsMax.z
    };

    for (int i = 0; i < 3; ++i)
    {
        if (std::fabs(direction[i]) < 0.000001f)
        {
            if (origin[i] < minBounds[i] ||
                origin[i] > maxBounds[i])
            {
                return false;
            }

            continue;
        }

        float inverseDirection =
            1.0f / direction[i];

        float t1 =
            (minBounds[i] - origin[i]) *
            inverseDirection;

        float t2 =
            (maxBounds[i] - origin[i]) *
            inverseDirection;

        if (t1 > t2)
        {
            std::swap(t1, t2);
        }

        if (t1 > tMin)
        {
            tMin = t1;
        }

        if (t2 < tMax)
        {
            tMax = t2;
        }

        if (tMin > tMax)
        {
            return false;
        }
    }

    distance = tMin;

    return true;
}

Entity* EditorLayer::PickEntity(
    const Ray& ray) const
{
    if (!m_Context)
    {
        return nullptr;
    }


    Entity* closestEntity = nullptr;

    float closestDistance =
        1000000.0f;


    for (const auto& entity : m_Context->GetEntities())
    {
        if (!entity)
        {
            continue;
        }

        const bool hasMeshRenderer =
            entity->HasComponent<
                MeshRendererComponent>();

        const bool hasModelRenderer =
            entity->HasComponent<
                ModelRendererComponent>();

        if (!hasMeshRenderer &&
            !hasModelRenderer)
        {
            continue;
        }

        const Matrix4 worldMatrix =
            entity->GetTransform().GetWorldMatrix();


        Vector3 localMin
        {
            -0.5f,
            -0.5f,
            -0.5f
        };

        Vector3 localMax
        {
            0.5f,
            0.5f,
            0.5f
        };

        if (entity->HasComponent<MeshRendererComponent>())
        {
            MeshRendererComponent& meshRenderer =
                entity->GetComponent<MeshRendererComponent>();

            Mesh* mesh =
                meshRenderer.GetMesh();

            if (mesh)
            {
                localMin =
                    mesh->GetBoundsMin();

                localMax =
                    mesh->GetBoundsMax();
            }
        }

        if (entity->HasComponent<ModelRendererComponent>())
        {
            ModelRendererComponent& modelRenderer =
                entity->GetComponent<ModelRendererComponent>();

            Model* model =
                modelRenderer.GetModel();

            if (model)
            {
                localMin =
                    model->GetBoundsMin();

                localMax =
                    model->GetBoundsMax();
            }
        }


        Vector3 corners[8]
        {
            { localMin.x, localMin.y, localMin.z },
            { localMax.x, localMin.y, localMin.z },
            { localMin.x, localMax.y, localMin.z },
            { localMax.x, localMax.y, localMin.z },

            { localMin.x, localMin.y, localMax.z },
            { localMax.x, localMin.y, localMax.z },
            { localMin.x, localMax.y, localMax.z },
            { localMax.x, localMax.y, localMax.z }
        };


        Vector3 worldMin
        {
            1000000.0f,
            1000000.0f,
            1000000.0f
        };


        Vector3 worldMax
        {
            -1000000.0f,
            -1000000.0f,
            -1000000.0f
        };


        for (const Vector3& corner : corners)
        {
            Vector3 worldCorner =
                worldMatrix.TransformPoint(corner);


            worldMin.x =
                std::min(
                    worldMin.x,
                    worldCorner.x);

            worldMin.y =
                std::min(
                    worldMin.y,
                    worldCorner.y);

            worldMin.z =
                std::min(
                    worldMin.z,
                    worldCorner.z);


            worldMax.x =
                std::max(
                    worldMax.x,
                    worldCorner.x);

            worldMax.y =
                std::max(
                    worldMax.y,
                    worldCorner.y);

            worldMax.z =
                std::max(
                    worldMax.z,
                    worldCorner.z);
        }


        float distance = 0.0f;


        if (RayIntersectsAABB(
                ray,
                worldMin,
                worldMax,
                distance))
        {
            if (distance < closestDistance)
            {
                closestDistance =
                    distance;

                closestEntity =
                    entity.get();
            }
        }
    }


    return closestEntity;
}

void EditorLayer::HandleAssetDrop(
    const std::string& assetPath)
{
    std::filesystem::path path(assetPath);

    if (!std::filesystem::exists(path))
    {
        std::cout
            << "Asset no encontrado: "
            << assetPath
            << '\n';

        return;
    }

    const std::string extension =
        path.extension().string();

    if (extension != ".obj" &&
        extension != ".gltf" &&
        extension != ".glb")
    {
        std::cout
            << "Asset no soportado para Model Drag & Drop: "
            << assetPath
            << '\n';

        return;
    }

    if (!m_Context)
        return;

    // ----------------------------------------------------
    // MODEL ASSET NAME
    // ----------------------------------------------------

    const std::string modelName =
        path.stem().string();

    // ----------------------------------------------------
    // LOAD MODEL ONLY IF IT DOES NOT EXIST
    // ----------------------------------------------------

    Model* model =
        AssetManager::GetModel(modelName);

    if (!model)
    {
        if (!AssetManager::LoadModel(
                modelName,
                assetPath))
        {
            std::cout
                << "ERROR: no se pudo importar modelo: "
                << assetPath
                << '\n';

            return;
        }

        model =
            AssetManager::GetModel(modelName);
    }

    if (!model)
    {
        std::cout
            << "ERROR: modelo no disponible: "
            << modelName
            << '\n';

        return;
    }

    // ----------------------------------------------------
    // UNIQUE ENTITY NAME
    // ----------------------------------------------------

    std::string entityName =
        modelName;

    int suffix = 1;

    while (true)
    {
        bool exists = false;

        for (const auto& existingEntity :
             m_Context->GetEntities())
        {
            if (existingEntity &&
                existingEntity->GetName() ==
                    entityName)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
            break;

        entityName =
            modelName +
            "_" +
            std::to_string(suffix++);
    }

    // ----------------------------------------------------
    // CREATE ENTITY
    // ----------------------------------------------------

    Entity& entity =
        m_Context->CreateEntity(
            entityName);

    entity.AddComponent<
        ModelRendererComponent>();

    entity.AddComponent<
        MaterialComponent>();

    entity.GetComponent<
        ModelRendererComponent>()
        .SetModel(model);

    // ----------------------------------------------------
    // MATERIAL
    // ----------------------------------------------------

    Material* material =
        model->GetMeshes().empty()
            ? nullptr
            : model->GetMeshes()[0]->GetMaterial();

    if (material)
    {
        entity.GetComponent<
            MaterialComponent>()
            .SetMaterial(material);
    }

    // ----------------------------------------------------
    // SELECT
    // ----------------------------------------------------

    m_SelectedEntity =
        &entity;

    std::cout
        << "Modelo añadido a la escena: "
        << entityName
        << '\n';
}

}