#pragma once

#include "AtlasEngine/Scene/Component.h"
#include "AtlasEngine/Math/Vector3.h"

namespace AtlasEngine
{

class DirectionalLightComponent : public Component
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

private:

    Vector3 m_Direction
    {
        -0.5f,
        -1.0f,
        -0.3f
    };

    Vector3 m_Color
    {
        1.0f,
        1.0f,
        1.0f
    };

    float m_Intensity = 1.0f;
};

}