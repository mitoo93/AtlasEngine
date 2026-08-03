#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Gameplay/GameLayer.h"
#include "AtlasEngine/Editor/EditorLayer.h"

int main()
{
    AtlasEngine::Application app;

    app.PushLayer(new AtlasEngine::GameLayer());
    app.PushOverlay(new AtlasEngine::EditorLayer());

    app.Run();

    return 0;
}