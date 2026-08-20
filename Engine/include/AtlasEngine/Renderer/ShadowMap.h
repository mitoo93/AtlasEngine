#pragma once

#include <cstdint>

namespace AtlasEngine
{

class ShadowMap
{
public:

    ShadowMap() = default;
    ~ShadowMap();

    bool Create(
        uint32_t width,
        uint32_t height);

    void BindForWriting() const;
    void Unbind() const;

    void BindTexture(
        uint32_t slot) const;

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

private:

    uint32_t m_Framebuffer = 0;
    uint32_t m_DepthTexture = 0;

    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
};

}