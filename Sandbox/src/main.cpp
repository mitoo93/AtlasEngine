#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Gameplay/GameLayer.h"
#include "AtlasEngine/Editor/EditorLayer.h"

int main()
{
    AtlasEngine::Application app;

    auto* gameLayer = new AtlasEngine::GameLayer();
    auto* editorLayer = new AtlasEngine::EditorLayer();

    app.PushLayer(gameLayer);
    app.PushOverlay(editorLayer);

    app.Run();

    return 0;
}