#include <glad/glad.h>

#include "AtlasEngine/Renderer/IndexBuffer.h"

namespace AtlasEngine
{

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::~IndexBuffer()
{
    if (m_RendererID)
        glDeleteBuffers(1, &m_RendererID);
}

bool IndexBuffer::Create(
    const unsigned int* indices,
    unsigned int count)
{
    m_Count = count;

    glGenBuffers(1, &m_RendererID);

    if (m_RendererID == 0)
        return false;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        count * sizeof(unsigned int),
        indices,
        GL_STATIC_DRAW);

    return true;
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}