#pragma once

#include "AtlasEngine/Scene/Component.h"
#include "AtlasEngine/Math/Vector3.h"

namespace AtlasEngine
{

class AmbientLightComponent : public Component
{
public:

    void SetColor(
        const Vector3& color)
    {
        m_Color = color;
    }

    const Vector3& GetColor() const
    {
        return m_Color;
    }

    void SetIntensity(
        float intensity)
    {
        m_Intensity = intensity;
    }

    float GetIntensity() const
    {
        return m_Intensity;
    }

private:

    Vector3 m_Color
    {
        1.0f,
        1.0f,
        1.0f
    };

    float m_Intensity = 0.2f;
};

}