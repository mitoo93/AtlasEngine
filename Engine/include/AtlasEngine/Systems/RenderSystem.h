#pragma once

#include "AtlasEngine/Systems/System.h"
#include "AtlasEngine/Math/Matrix4.h"

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
        const Matrix4& projection);

    void ClearViewProjection();

private:

    bool m_UseViewProjectionOverride = false;

    Matrix4 m_ViewOverride;
    Matrix4 m_ProjectionOverride;
};

}