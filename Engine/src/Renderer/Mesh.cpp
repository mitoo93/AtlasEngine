#include <glad/glad.h>

#include "AtlasEngine/Renderer/Mesh.h"

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
    m_VertexArray.Create();

    m_VertexBuffer.Create(
        vertices.data(),
        static_cast<unsigned int>(vertices.size() * sizeof(Vertex)));

    m_VertexBuffer.SetLayout(layout);

    m_IndexBuffer.Create(
        indices.data(),
        static_cast<unsigned int>(indices.size()));

    m_VertexArray.AddVertexBuffer(m_VertexBuffer);

    m_IndexBuffer.Bind();

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

}