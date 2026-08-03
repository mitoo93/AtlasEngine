#pragma once

#include "AtlasEngine/Core/Layer.h"

namespace AtlasEngine
{

class EditorLayer : public Layer
{
public:

    EditorLayer()
        : Layer("EditorLayer")
    {
    }

    void OnAttach() override;

    void OnUpdate(float dt) override;

    void OnDetach() override;
};

}