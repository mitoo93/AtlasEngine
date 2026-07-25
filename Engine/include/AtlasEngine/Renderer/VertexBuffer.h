#pragma once

namespace AtlasEngine
{

class VertexBuffer
{
public:

    VertexBuffer();
    ~VertexBuffer();

    bool Create(const void* data, unsigned int size);

    void Bind() const;
    void Unbind() const;

private:

    unsigned int m_RendererID = 0;
};

}