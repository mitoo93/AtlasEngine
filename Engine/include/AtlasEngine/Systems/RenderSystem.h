#pragma once

#include "AtlasEngine/Systems/System.h"
#include "AtlasEngine/Math/Matrix4.h"
#include "AtlasEngine/Math/Vector3.h"
#include "AtlasEngine/Renderer/ShadowMap.h"

namespace AtlasEngine
{

class Shader;

class RenderSystem : public System
{
public:

    void Update(
        Scene& scene) override;

    void SetViewProjection(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition);

    void ClearViewProjection();

private:

    bool m_UseViewProjectionOverride = false;

    Matrix4 m_ViewOverride;
    Matrix4 m_ProjectionOverride;

    Vector3 m_CameraPositionOverride
    {
        0.0f,
        0.0f,
        0.0f
    };

    ShadowMap m_ShadowMap;
        bool m_ShadowMapInitialized = false;

    uint32_t m_ShadowMapResolution =
        2048;

    float m_ShadowOrthoSize =
        5.0f;

    float m_ShadowNearPlane =
        0.1f;

    float m_ShadowFarPlane =
        30.0f;

    float m_ShadowLightDistance =
        10.0f;
};

}