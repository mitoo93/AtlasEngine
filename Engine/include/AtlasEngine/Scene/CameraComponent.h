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

private:

    Camera m_Camera;
};

}