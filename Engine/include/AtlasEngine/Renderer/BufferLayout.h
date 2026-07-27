#pragma once

#include <vector>

namespace AtlasEngine
{

struct BufferElement
{
    unsigned int Type;
    unsigned int Count;
    bool Normalized;

    unsigned int Size;
    unsigned int Offset;
};

class BufferLayout
{
public:

    void PushFloat(unsigned int count);

    const std::vector<BufferElement>& GetElements() const
    {
        return m_Elements;
    }

    unsigned int GetStride() const
    {
        return m_Stride;
    }

private:

    void CalculateOffsetsAndStride();

private:

    std::vector<BufferElement> m_Elements;

    unsigned int m_Stride = 0;
};

}