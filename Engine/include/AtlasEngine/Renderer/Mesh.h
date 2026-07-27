#pragma once

#include "AtlasEngine/Renderer/VertexArray.h"
#include "AtlasEngine/Renderer/VertexBuffer.h"
#include "AtlasEngine/Renderer/IndexBuffer.h"
#include "AtlasEngine/Renderer/BufferLayout.h"

namespace AtlasEngine
{

class Mesh
{
public:

    Mesh();
    ~Mesh();

    bool Create(
        const void* vertices,
        unsigned int vertexSize,
        const unsigned int* indices,
        unsigned int indexCount,
        const BufferLayout& layout);

    void Bind() const;

    void Draw() const;

private:

    VertexArray m_VertexArray;
    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;
};

}