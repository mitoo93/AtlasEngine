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
    const void* vertices,
    unsigned int vertexSize,
    const unsigned int* indices,
    unsigned int indexCount,
    const BufferLayout& layout)

{
    m_VertexArray.Create();

    m_VertexBuffer.Create(
        vertices,
        vertexSize);

    m_VertexBuffer.SetLayout(layout);

    m_IndexBuffer.Create(
        indices,
        indexCount);

    m_VertexArray.Bind();
    m_VertexBuffer.Bind();
    m_IndexBuffer.Bind();

    const BufferLayout& bufferLayout = m_VertexBuffer.GetLayout();

    unsigned int index = 0;

    for (const auto& element : bufferLayout.GetElements())
    {
        glVertexAttribPointer(
            index,
            element.Count,
            element.Type,
            element.Normalized ? GL_TRUE : GL_FALSE,
            bufferLayout.GetStride(),
            reinterpret_cast<const void*>(element.Offset));

        glEnableVertexAttribArray(index);

        ++index;
    }

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

}