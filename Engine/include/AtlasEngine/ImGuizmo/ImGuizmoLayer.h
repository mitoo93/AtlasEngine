#pragma once

#include <imgui.h>

namespace AtlasEngine
{

class Scene;
class Entity;

class ImGuizmoLayer
{
public:

    void OnImGuiRender(
        Entity* selectedEntity,
        Scene* scene,
        const ImVec2& viewportPosition,
        const ImVec2& viewportSize);
};

}