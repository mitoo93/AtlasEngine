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
    unsigned int indexCount)
{
    m_VertexArray.Create();

    m_VertexBuffer.Create(
        vertices,
        vertexSize);

    m_IndexBuffer.Create(
        indices,
        indexCount);

    m_VertexArray.Bind();
    m_VertexBuffer.Bind();
    m_IndexBuffer.Bind();

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(float) * 3,
        (void*)0);

    glEnableVertexAttribArray(0);

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