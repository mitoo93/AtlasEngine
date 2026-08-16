#include "AtlasEngine/Renderer/Model.h"

#include "AtlasEngine/Renderer/Mesh.h"

namespace AtlasEngine
{

void Model::AddMesh(
    std::unique_ptr<Mesh> mesh)
{
    if (!mesh)
        return;

    m_Meshes.push_back(
        std::move(mesh));
}

const std::vector<
    std::unique_ptr<Mesh>>&
Model::GetMeshes() const
{
    return m_Meshes;
}

void Model::SetBounds(
    const Vector3& min,
    const Vector3& max)
{
    m_BoundsMin = min;
    m_BoundsMax = max;
}

const Vector3& Model::GetBoundsMin() const
{
    return m_BoundsMin;
}

const Vector3& Model::GetBoundsMax() const
{
    return m_BoundsMax;
}

}