#pragma once

#include "AtlasEngine/Renderer/VertexBuffer.h"

namespace AtlasEngine
{

class VertexArray
{
public:

    VertexArray();
    ~VertexArray();

    bool Create();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const VertexBuffer& vertexBuffer);

private:

    unsigned int m_RendererID = 0;
    unsigned int m_NextAttributeIndex = 0;
};

}