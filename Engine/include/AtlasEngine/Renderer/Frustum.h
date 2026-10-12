#pragma once

#include "AtlasEngine/Math/Matrix4.h"
#include "AtlasEngine/Math/Vector3.h"

namespace AtlasEngine
{

struct FrustumPlane
{
    Vector3 Normal
    {
        0.0f,
        0.0f,
        0.0f
    };

    float Distance =
        0.0f;

    void Normalize();
};


class Frustum
{
public:

    void Update(
        const Matrix4& viewProjection);

    bool IntersectsAABB(
        const Vector3& boundsMin,
        const Vector3& boundsMax) const;

private:

    enum PlaneIndex
    {
        Left = 0,
        Right,
        Bottom,
        Top,
        Near,
        Far,

        Count
    };

    FrustumPlane m_Planes[Count];
};

}