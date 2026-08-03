#pragma once

#include "AtlasEngine/Core/Layer.h"
#include "AtlasEngine/Scene/Scene.h"
#include "AtlasEngine/Controllers/CameraController.h"

namespace AtlasEngine
{

class GameLayer : public Layer
{
public:

    GameLayer()
        : Layer("GameLayer")
    {
    }

    void OnAttach() override;
    void OnUpdate(float dt) override;
    void OnDetach() override;

private:

    Scene m_Scene;

    CameraController m_CameraController;

    bool m_FirstFrame = true;
};

}