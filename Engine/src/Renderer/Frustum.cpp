#include "AtlasEngine/Renderer/Frustum.h"

#include <cmath>

namespace AtlasEngine
{

void FrustumPlane::Normalize()
{
    float length =
        std::sqrt(
            Normal.x * Normal.x +
            Normal.y * Normal.y +
            Normal.z * Normal.z);

    if (length <= 0.000001f)
    {
        return;
    }

    float inverseLength =
        1.0f /
        length;

    Normal.x *=
        inverseLength;

    Normal.y *=
        inverseLength;

    Normal.z *=
        inverseLength;

    Distance *=
        inverseLength;
}


void Frustum::Update(
    const Matrix4& viewProjection)
{
    const float* m =
        viewProjection.Data();


    // ----------------------------------------------------
    // LEFT
    // row 3 + row 0
    // ----------------------------------------------------

    m_Planes[Left].Normal =
    {
        m[3] + m[0],
        m[7] + m[4],
        m[11] + m[8]
    };

    m_Planes[Left].Distance =
        m[15] +
        m[12];


    // ----------------------------------------------------
    // RIGHT
    // row 3 - row 0
    // ----------------------------------------------------

    m_Planes[Right].Normal =
    {
        m[3] - m[0],
        m[7] - m[4],
        m[11] - m[8]
    };

    m_Planes[Right].Distance =
        m[15] -
        m[12];


    // ----------------------------------------------------
    // BOTTOM
    // row 3 + row 1
    // ----------------------------------------------------

    m_Planes[Bottom].Normal =
    {
        m[3] + m[1],
        m[7] + m[5],
        m[11] + m[9]
    };

    m_Planes[Bottom].Distance =
        m[15] +
        m[13];


    // ----------------------------------------------------
    // TOP
    // row 3 - row 1
    // ----------------------------------------------------

    m_Planes[Top].Normal =
    {
        m[3] - m[1],
        m[7] - m[5],
        m[11] - m[9]
    };

    m_Planes[Top].Distance =
        m[15] -
        m[13];


    // ----------------------------------------------------
    // NEAR
    // row 3 + row 2
    //
    // OpenGL clip space:
    // -w <= z <= w
    // ----------------------------------------------------

    m_Planes[Near].Normal =
    {
        m[3] + m[2],
        m[7] + m[6],
        m[11] + m[10]
    };

    m_Planes[Near].Distance =
        m[15] +
        m[14];


    // ----------------------------------------------------
    // FAR
    // row 3 - row 2
    // ----------------------------------------------------

    m_Planes[Far].Normal =
    {
        m[3] - m[2],
        m[7] - m[6],
        m[11] - m[10]
    };

    m_Planes[Far].Distance =
        m[15] -
        m[14];


    for (int i = 0;
         i < Count;
         ++i)
    {
        m_Planes[i].Normalize();
    }
}


bool Frustum::IntersectsAABB(
    const Vector3& boundsMin,
    const Vector3& boundsMax) const
{
    for (int i = 0;
         i < Count;
         ++i)
    {
        const FrustumPlane& plane =
            m_Planes[i];


        Vector3 positiveVertex
        {
            plane.Normal.x >= 0.0f
                ? boundsMax.x
                : boundsMin.x,

            plane.Normal.y >= 0.0f
                ? boundsMax.y
                : boundsMin.y,

            plane.Normal.z >= 0.0f
                ? boundsMax.z
                : boundsMin.z
        };


        float distance =
            plane.Normal.x *
                positiveVertex.x +
            plane.Normal.y *
                positiveVertex.y +
            plane.Normal.z *
                positiveVertex.z +
            plane.Distance;


        if (distance < 0.0f)
        {
            return false;
        }
    }


    return true;
}

}