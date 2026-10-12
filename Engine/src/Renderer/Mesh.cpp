#include <glad/glad.h>

#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Material.h"

#include <iostream>
#include <limits>

namespace AtlasEngine
{

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    if (m_InstanceBuffer)
    {
        glDeleteBuffers(
            1,
            &m_InstanceBuffer);
    }
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

bool Mesh::DrawInstanced(
    const std::vector<float>& instanceMatrices)
{
    constexpr std::size_t floatsPerMatrix = 16;

    if (instanceMatrices.empty() ||
        instanceMatrices.size() % floatsPerMatrix != 0)
    {
        return false;
    }

    const std::size_t instanceCount =
        instanceMatrices.size() / floatsPerMatrix;

    if (instanceCount >
        static_cast<std::size_t>(
            (std::numeric_limits<GLsizei>::max)()))
    {
        return false;
    }

    if (instanceMatrices.size() >
        static_cast<std::size_t>(
            (std::numeric_limits<GLsizeiptr>::max)()) /
            sizeof(float))
    {
        return false;
    }

    if (!m_InstanceBuffer)
    {
        glGenBuffers(
            1,
            &m_InstanceBuffer);

        if (!m_InstanceBuffer)
        {
            return false;
        }
    }

    Bind();

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_InstanceBuffer);

    const GLsizeiptr byteSize =
        static_cast<GLsizeiptr>(
            instanceMatrices.size() * sizeof(float));

    glBufferData(
        GL_ARRAY_BUFFER,
        byteSize,
        instanceMatrices.data(),
        GL_STREAM_DRAW);

    const GLsizei stride =
        static_cast<GLsizei>(
            floatsPerMatrix * sizeof(float));

    for (GLuint column = 0; column < 4; ++column)
    {
        const GLuint location = 5 + column;

        const std::uintptr_t offset =
            column * 4 * sizeof(float);

        glEnableVertexAttribArray(location);

        glVertexAttribPointer(
            location,
            4,
            GL_FLOAT,
            GL_FALSE,
            stride,
            reinterpret_cast<const void*>(offset));

        glVertexAttribDivisor(
            location,
            1);
    }

    glDrawElementsInstanced(
        GL_TRIANGLES,
        m_IndexBuffer.GetCount(),
        GL_UNSIGNED_INT,
        nullptr,
        static_cast<GLsizei>(instanceCount));

    // Restaura los atributos para dibujos individuales
    // y para otros pases que utilicen esta misma Mesh.
    for (GLuint column = 0; column < 4; ++column)
    {
        const GLuint location = 5 + column;

        glDisableVertexAttribArray(location);

        glVertexAttribDivisor(
            location,
            0);
    }

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0);

    return true;
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