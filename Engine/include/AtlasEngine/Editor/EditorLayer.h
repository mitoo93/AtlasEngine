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

    enum class EditorState
    {
        Edit = 0,
        Play
    };

    void OnAttach() override;

    void OnUpdate(float dt) override;

    void OnDetach() override;

    void OnImGuiRender() override;

    void SetContext(Scene* scene);

    EditorState GetState() const
    {
        return m_State;
    }

    bool IsPlaying() const
    {
        return m_State == EditorState::Play;
    }

    bool IsEditing() const
    {
        return m_State == EditorState::Edit;
    }

    const ImVec2& GetViewportSize() const
    {
        return m_ViewportSize;
    }

private:

    Scene* m_Context = nullptr;

    Entity* m_SelectedEntity = nullptr;

    EditorState m_State = EditorState::Edit;

    ImVec2 m_ViewportSize = { 0.0f, 0.0f };
};

} // namespace AtlasEngine