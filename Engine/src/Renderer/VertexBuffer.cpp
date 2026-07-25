#include <glad/glad.h>

#include "AtlasEngine/Renderer/VertexBuffer.h"

namespace AtlasEngine
{

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::~VertexBuffer()
{
    if (m_RendererID)
        glDeleteBuffers(1, &m_RendererID);
}

bool VertexBuffer::Create(
    const void* data,
    unsigned int size)
{
    glGenBuffers(1, &m_RendererID);

    if (m_RendererID == 0)
        return false;

    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

    glBufferData(
        GL_ARRAY_BUFFER,
        size,
        data,
        GL_STATIC_DRAW);

    return true;
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace AtlasEngine


