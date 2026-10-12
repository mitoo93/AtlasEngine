#pragma once

#include "AtlasEngine/Systems/System.h"
#include "AtlasEngine/Math/Matrix4.h"
#include "AtlasEngine/Math/Vector3.h"
#include "AtlasEngine/Renderer/ShadowMap.h"
#include "AtlasEngine/Renderer/Frustum.h"
#include "AtlasEngine/Renderer/RenderItem.h"

#include <vector>
#include <cstdint>

namespace AtlasEngine
{

class Shader;
class Material;
class Cubemap;
class BRDFLUT;
class Mesh;
class Scene;

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


    bool CalculateWorldBounds(
        const Vector3& localMin,
        const Vector3& localMax,
        const Matrix4& worldMatrix,
        Vector3& worldMin,
        Vector3& worldMax) const;


    void BuildRenderQueue(
        Scene& scene,
        const Frustum& cameraFrustum);


    void SortRenderQueue();


    void ExecuteRenderQueue(
        const Matrix4& view,
        const Matrix4& projection,
        const Matrix4& lightSpaceMatrix,
        const Vector3& cameraPosition,
        DirectionalLightComponent* directionalLight,
        AmbientLightComponent* ambientLight,
        PointLightComponent* const* pointLights,
        const Vector3* pointLightPositions,
        int pointLightCount,
        SpotLightComponent* spotLight,
        const Vector3& spotLightPosition,
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

    struct ShadowItem
    {
        Mesh* MeshPtr = nullptr;
        Matrix4 ModelMatrix;
    };

    std::vector<RenderItem> m_RenderQueue;

    std::vector<ShadowItem> m_ShadowQueue;

    std::vector<float> m_ShadowInstanceMatrices;

    std::vector<float> m_SceneInstanceMatrices;
};

}