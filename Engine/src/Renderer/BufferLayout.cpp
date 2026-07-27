#include "AtlasEngine/Renderer/BufferLayout.h"

#include <glad/glad.h>


namespace AtlasEngine
{

void BufferLayout::PushFloat(unsigned int count)
{
    m_Elements.push_back(BufferElement{
        GL_FLOAT,
        count,
        false,
        static_cast<unsigned int>(count * sizeof(float)),
        0
    });

    CalculateOffsetsAndStride();
}

void BufferLayout::CalculateOffsetsAndStride()
{
    unsigned int offset = 0;

    m_Stride = 0;

    for (auto& element : m_Elements)
    {
        element.Offset = offset;

        offset += element.Size;

        m_Stride += element.Size;
    }
}
}