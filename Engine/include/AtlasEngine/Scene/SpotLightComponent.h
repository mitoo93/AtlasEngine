#pragma once

#include "AtlasEngine/Scene/Component.h"
#include "AtlasEngine/Math/Vector3.h"

namespace AtlasEngine
{

class SpotLightComponent : public Component
{
public:

    void SetDirection(
        const Vector3& direction)
    {
        m_Direction = direction;
    }

    const Vector3& GetDirection() const
    {
        return m_Direction;
    }

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

    void SetInnerCutoff(
        float cutoff)
    {
        m_InnerCutoff = cutoff;
    }

    float GetInnerCutoff() const
    {
        return m_InnerCutoff;
    }

    void SetOuterCutoff(
        float cutoff)
    {
        m_OuterCutoff = cutoff;
    }

    float GetOuterCutoff() const
    {
        return m_OuterCutoff;
    }

private:

    Vector3 m_Direction
    {
        0.0f,
        0.0f,
        -1.0f
    };

    Vector3 m_Color
    {
        1.0f,
        1.0f,
        1.0f
    };

    float m_Intensity = 1.0f;

    float m_InnerCutoff = 12.5f;
    float m_OuterCutoff = 17.5f;
};

}