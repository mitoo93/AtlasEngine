#include <glad/glad.h>

#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Material.h"

#include <iostream>

namespace AtlasEngine
{

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::Create(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    const BufferLayout& layout)
{
    m_VertexCount =
        static_cast<uint32_t>(
            vertices.size());

    m_IndexCount =
        static_cast<uint32_t>(
            indices.size());
    
    m_VertexArray.Create();

    // IMPORTANTE:
    // El VAO correcto debe estar activo antes
    // de crear/enlazar el IndexBuffer.
    m_VertexArray.Bind();

    m_VertexBuffer.Create(
        vertices.data(),
        static_cast<unsigned int>(
            vertices.size() * sizeof(Vertex)));

    m_VertexBuffer.SetLayout(
        layout);

    m_IndexBuffer.Create(
        indices.data(),
        static_cast<unsigned int>(
            indices.size()));

    m_VertexArray.AddVertexBuffer(
        m_VertexBuffer);

    m_IndexBuffer.Bind();

    m_VertexArray.Unbind();

    return true;
}

void Mesh::Bind() const
{
    m_VertexArray.Bind();
}

void Mesh::Draw() const
{
    Bind();

    glDrawElements(
        GL_TRIANGLES,
        m_IndexBuffer.GetCount(),
        GL_UNSIGNED_INT,
        nullptr);
}

void Mesh::SetName(
    const std::string& name)
{
    m_Name = name;
}

const std::string& Mesh::GetName() const
{
    return m_Name;
}

void Mesh::SetMaterial(
    Material* material)
{
    m_Material = material;
}

Material* Mesh::GetMaterial()
{
    return m_Material;
}

const Material* Mesh::GetMaterial() const
{
    return m_Material;
}

void Mesh::SetBounds(
    const Vector3& min,
    const Vector3& max)
{
    m_BoundsMin = min;
    m_BoundsMax = max;
}

const Vector3& Mesh::GetBoundsMin() const
{
    return m_BoundsMin;
}

const Vector3& Mesh::GetBoundsMax() const
{
    return m_BoundsMax;
}

}