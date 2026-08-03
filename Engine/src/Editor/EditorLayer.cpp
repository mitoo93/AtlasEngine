#include "AtlasEngine/Editor/EditorLayer.h"

#include <iostream>

namespace AtlasEngine
{

void EditorLayer::OnAttach()
{
    std::cout << "EditorLayer Attach\n";
}

void EditorLayer::OnUpdate(float)
{
}

void EditorLayer::OnDetach()
{
    std::cout << "EditorLayer Detach\n";
}

}