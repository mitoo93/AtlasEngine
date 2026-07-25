#pragma once

namespace AtlasEngine
{

class IndexBuffer
{
public:

    IndexBuffer();
    ~IndexBuffer();

    unsigned int GetCount() const
    {
        return m_Count;
    }

    bool Create(const unsigned int* indices,
                unsigned int count);

    void Bind() const;
    void Unbind() const;

private:

    unsigned int m_RendererID = 0;
    unsigned int m_Count = 0;
};

}