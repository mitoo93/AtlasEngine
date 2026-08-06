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

private:

    Scene* m_Context = nullptr;

    Entity* m_SelectedEntity = nullptr;

    EditorState m_State = EditorState::Edit;
};

} // namespace AtlasEngine