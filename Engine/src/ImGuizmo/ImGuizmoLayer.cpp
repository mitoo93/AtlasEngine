#include "AtlasEngine/ImGuizmo/ImGuizmoLayer.h"

#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Scene/Entity.h"
#include "AtlasEngine/Scene/CameraComponent.h"

#include "AtlasEngine/Math/Transform.h"

#include <ImGuizmo.h>

#include <iostream>

namespace AtlasEngine
{

void ImGuizmoLayer::OnImGuiRender(
    Entity* selectedEntity,
    Scene* scene,
    const ImVec2& viewportPosition,
    const ImVec2& viewportSize)
{
    std::cout << "ImGuizmo\n";
    
    if (!selectedEntity)
    {
        std::cout << "NO selectedEntity\n";
        return;
    }

    if (!scene)
    {
        std::cout << "NO scene\n";
        return;
    }

    CameraComponent* camera =
        scene->FindPrimaryCamera();

    if (!camera)
    {
        std::cout << "NO primary camera\n";
        return;
    }

    if (viewportSize.x <= 0.0f ||
        viewportSize.y <= 0.0f)
    {
        std::cout << "Viewport size invalid\n";
        return;
    }

    std::cout << "PASO TODOS LOS FILTROS\n";

    ImGuizmo::SetOrthographic(false);

    ImGuizmo::BeginFrame();

    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(
        viewportPosition.x,
        viewportPosition.y,
        viewportSize.x,
        viewportSize.y);

    Transform& transform =
        selectedEntity->GetTransform();

    Matrix4 model =
        transform.GetWorldMatrix();

    Matrix4 view =
        camera->GetCamera().GetViewMatrix();

    Matrix4 projection =
        camera->GetCamera().GetProjectionMatrix();

    ImGuizmo::Enable(true);
    
    bool over = ImGuizmo::IsOver();

    ImGui::Begin("Debug");
    ImGui::Text("IsOver = %d", over);
    ImGui::End();

    std::cout
        << "Viewport: "
        << viewportPosition.x << ", "
        << viewportPosition.y
        << " -> "
        << viewportSize.x << "x"
        << viewportSize.y
        << '\n';

    std::cout
        << "Camera Pos: "
        << camera->GetCamera().GetPosition().x << " "
        << camera->GetCamera().GetPosition().y << " "
        << camera->GetCamera().GetPosition().z
        << '\n';

    std::cout
        << "Entity Pos: "
        << transform.GetPosition().x << " "
        << transform.GetPosition().y << " "
        << transform.GetPosition().z
        << '\n';

    std::cout << "View Matrix\n";

    for (int i = 0; i < 16; i++)
    {
        std::cout << view.Data()[i] << " ";

        if ((i + 1) % 4 == 0)
            std::cout << '\n';
    }

    std::cout << '\n';

    std::cout << "Projection Matrix\n";

    for (int i = 0; i < 16; i++)
    {
        std::cout << projection.Data()[i] << " ";

        if ((i + 1) % 4 == 0)
            std::cout << '\n';
    }

    std::cout << '\n';

    std::cout << "Model Matrix\n";

    for (int i = 0; i < 16; i++)
    {
        std::cout << model.Data()[i] << " ";

        if ((i + 1) % 4 == 0)
            std::cout << '\n';
    }

    std::cout << '\n';

    ImGuizmo::SetID(0);

    std::cout << "IsUsing = " << ImGuizmo::IsUsing() << '\n';
    
    ImGuizmo::Manipulate(
        view.Data(),
        projection.Data(),
        ImGuizmo::TRANSLATE,
        ImGuizmo::LOCAL,
        model.Data());

    if (ImGuizmo::IsUsing())
    {
        float translation[3];
        float rotation[3];
        float scale[3];

        ImGuizmo::DecomposeMatrixToComponents(
            model.Data(),
            translation,
            rotation,
            scale);

        transform.SetPosition({
            translation[0],
            translation[1],
            translation[2]
        });

        transform.SetRotation({
            rotation[0],
            rotation[1],
            rotation[2]
        });

        transform.SetScale({
            scale[0],
            scale[1],
            scale[2]
        });
    }
}

}