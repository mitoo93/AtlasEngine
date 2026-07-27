#pragma once

#include "AtlasEngine/Renderer/BufferLayout.h"

namespace AtlasEngine
{

class VertexBuffer
{
public:

    VertexBuffer();
    ~VertexBuffer();

    bool Create(const void* data,
                unsigned int size);

    void Bind() const;
    void Unbind() const;

    void SetLayout(const BufferLayout& layout)
    {
        m_Layout = layout;
    }

    const BufferLayout& GetLayout() const
    {
        return m_Layout;
    }

private:

    unsigned int m_RendererID = 0;

    BufferLayout m_Layout;
};

} // namespace AtlasEngine