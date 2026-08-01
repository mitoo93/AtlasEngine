#pragma once

#include "AtlasEngine/Scene/Component.h"
#include "AtlasEngine/Renderer/Camera.h"

namespace AtlasEngine
{

class CameraComponent : public Component
{
public:

    Camera& GetCamera()
    {
        return m_Camera;
    }

    const Camera& GetCamera() const
    {
        return m_Camera;
    }

    bool IsPrimary() const
    {
        return m_IsPrimary;
    }

    void SetPrimary(bool value)
    {
        m_IsPrimary = value;
    }

private:

    Camera m_Camera;

    bool m_IsPrimary = false;
};

}