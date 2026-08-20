#pragma once

#include "AtlasEngine/Scene/Component.h"
#include "AtlasEngine/Math/Vector3.h"

namespace AtlasEngine
{

class PointLightComponent : public Component
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

    void SetConstant(
        float constant)
    {
        m_Constant = constant;
    }

    float GetConstant() const
    {
        return m_Constant;
    }

    void SetLinear(
        float linear)
    {
        m_Linear = linear;
    }

    float GetLinear() const
    {
        return m_Linear;
    }

    void SetQuadratic(
        float quadratic)
    {
        m_Quadratic = quadratic;
    }

    float GetQuadratic() const
    {
        return m_Quadratic;
    }

private:

    Vector3 m_Color
    {
        1.0f,
        1.0f,
        1.0f
    };

    float m_Intensity = 1.0f;

    float m_Constant = 1.0f;
    float m_Linear = 0.09f;
    float m_Quadratic = 0.032f;
};

}