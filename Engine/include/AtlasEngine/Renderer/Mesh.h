#pragma once

#include "AtlasEngine/Renderer/VertexArray.h"
#include "AtlasEngine/Renderer/VertexBuffer.h"
#include "AtlasEngine/Renderer/IndexBuffer.h"
#include "AtlasEngine/Renderer/BufferLayout.h"
#include <vector>
#include "AtlasEngine/Renderer/Vertex.h"

namespace AtlasEngine
{

class Mesh
{
public:

    Mesh();
    ~Mesh();

    bool Create(
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const BufferLayout& layout);

    void Bind() const;

    void Draw() const;

private:

    VertexArray m_VertexArray;
    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;
};

}