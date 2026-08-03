#pragma once

#include <string>

#include "AtlasEngine/Events/Event.h"

namespace AtlasEngine
{

class Layer
{
public:

    Layer(
        const std::string& name = "Layer");

    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}

    virtual void OnUpdate(float deltaTime) {}

    virtual void OnEvent(Event& event) {}

    virtual void OnImGuiRender() {}

    const std::string& GetName() const;

private:

    std::string m_Name;
};

}