#pragma once

#include "AtlasEngine/Core/Layer.h"

namespace AtlasEngine
{

class Scene;

class Entity;

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

    void OnImGuiRender() override;

    void SetContext(Scene* scene);

private:

    Scene* m_Context = nullptr;

    Entity* m_SelectedEntity = nullptr;
};

}