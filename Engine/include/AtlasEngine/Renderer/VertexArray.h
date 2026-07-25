#pragma once

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

private:

    unsigned int m_RendererID = 0;
};

}