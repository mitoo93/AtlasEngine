#pragma once

#include "AtlasEngine/Math/Vector3.h"

#include <memory>
#include <vector>

namespace AtlasEngine
{

class Mesh;

class Model
{
public:

    Model() = default;
    ~Model() = default;

    void AddMesh(
        std::unique_ptr<Mesh> mesh);

    const std::vector<
        std::unique_ptr<Mesh>>&
        GetMeshes() const;

    void SetBounds(
        const Vector3& min,
        const Vector3& max);

    const Vector3& GetBoundsMin() const;

    const Vector3& GetBoundsMax() const;

private:

    std::vector<
        std::unique_ptr<Mesh>>
        m_Meshes;

    Vector3 m_BoundsMin
    {
        0.0f,
        0.0f,
        0.0f
    };

    Vector3 m_BoundsMax
    {
        0.0f,
        0.0f,
        0.0f
    };
};

}