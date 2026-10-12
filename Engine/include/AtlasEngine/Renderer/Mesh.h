#pragma once

#include "AtlasEngine/Renderer/VertexArray.h"
#include "AtlasEngine/Renderer/VertexBuffer.h"
#include "AtlasEngine/Renderer/IndexBuffer.h"
#include "AtlasEngine/Renderer/BufferLayout.h"
#include "AtlasEngine/Renderer/Vertex.h"
#include "AtlasEngine/Math/Vector3.h"

#include <vector>
#include <string>
#include <cstdint>

namespace AtlasEngine
{

class Material;

class Mesh
{
public:

    Mesh();
    ~Mesh();

    bool Create(
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const BufferLayout& layout);

    void Bind() const;

    void Draw() const;

    bool DrawInstanced(
        const std::vector<float>& instanceMatrices);

    void SetName(
        const std::string& name);

    const std::string& GetName() const;

    void SetMaterial(
        Material* material);

    Material* GetMaterial();

    const Material* GetMaterial() const;

    void SetBounds(
        const Vector3& min,
        const Vector3& max);

    const Vector3& GetBoundsMin() const;

    const Vector3& GetBoundsMax() const;

    uint32_t GetVertexCount() const
    {
        return m_VertexCount;
    }

    uint32_t GetIndexCount() const
    {
        return m_IndexCount;
    }

private:

    VertexArray m_VertexArray;
    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;

    std::string m_Name;

    Material* m_Material = nullptr;

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

    uint32_t m_VertexCount = 0;
    uint32_t m_IndexCount = 0;

    unsigned int m_InstanceBuffer = 0;
};

}