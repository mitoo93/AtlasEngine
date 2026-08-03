#pragma once

#include "AtlasEngine/Core/Layer.h"

namespace AtlasEngine
{

class ImGuiLayer : public Layer
{
public:

    ImGuiLayer();

    ~ImGuiLayer() override;

    virtual void OnAttach() override;

    virtual void OnDetach() override;

    void Begin();

    void End();
};

}