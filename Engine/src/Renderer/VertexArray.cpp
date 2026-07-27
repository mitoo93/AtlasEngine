#include <glad/glad.h>

#include "AtlasEngine/Renderer/VertexArray.h"

namespace AtlasEngine
{

VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
    if (m_RendererID)
        glDeleteVertexArrays(1, &m_RendererID);
}

bool VertexArray::Create()
{
    glGenVertexArrays(1, &m_RendererID);

    if (m_RendererID == 0)
        return false;

    return true;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer)
{
    Bind();

    vertexBuffer.Bind();

    const BufferLayout& layout = vertexBuffer.GetLayout();

    for (const auto& element : layout.GetElements())
    {
        glVertexAttribPointer(
            m_NextAttributeIndex,
            element.Count,
            element.Type,
            element.Normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            reinterpret_cast<const void*>(element.Offset));

        glEnableVertexAttribArray(m_NextAttributeIndex);

        ++m_NextAttributeIndex;
    }
}

} // namespace AtlasEngine