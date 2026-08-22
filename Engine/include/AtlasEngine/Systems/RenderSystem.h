#pragma once

#include "AtlasEngine/Systems/System.h"
#include "AtlasEngine/Math/Matrix4.h"
#include "AtlasEngine/Math/Vector3.h"
#include "AtlasEngine/Renderer/ShadowMap.h"

namespace AtlasEngine
{

class Shader;
class Material;
class Cubemap;
class BRDFLUT;

class DirectionalLightComponent;
class AmbientLightComponent;
class PointLightComponent;
class SpotLightComponent;


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

    void BindMaterial(
        Shader& shader,
        Material& material);


    void BindLights(
        Shader& shader,
        DirectionalLightComponent* directionalLight,
        AmbientLightComponent* ambientLight,
        PointLightComponent* const* pointLights,
        const Vector3* pointLightPositions,
        int pointLightCount,
        SpotLightComponent* spotLight,
        const Vector3& spotLightPosition,
        const Vector3& cameraPosition);


    void BindShadowAndIBL(
        Shader& shader,
        Cubemap* irradianceMap,
        Cubemap* prefilterMap,
        BRDFLUT* brdfLUT);


private:

    bool m_UseViewProjectionOverride =
        false;


    Matrix4 m_ViewOverride;

    Matrix4 m_ProjectionOverride;


    Vector3 m_CameraPositionOverride
    {
        0.0f,
        0.0f,
        0.0f
    };


    ShadowMap m_ShadowMap;


    bool m_ShadowMapInitialized =
        false;


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