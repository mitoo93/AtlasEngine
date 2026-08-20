#include "AtlasEngine/Renderer/ShadowMap.h"

#include <glad/glad.h>

#include <iostream>

namespace AtlasEngine
{

ShadowMap::~ShadowMap()
{
    if (m_DepthTexture)
    {
        glDeleteTextures(
            1,
            &m_DepthTexture);
    }

    if (m_Framebuffer)
    {
        glDeleteFramebuffers(
            1,
            &m_Framebuffer);
    }
}

bool ShadowMap::Create(
    uint32_t width,
    uint32_t height)
{
    m_Width =
        width;

    m_Height =
        height;

    glGenFramebuffers(
        1,
        &m_Framebuffer);

    glGenTextures(
        1,
        &m_DepthTexture);

    glBindTexture(
        GL_TEXTURE_2D,
        m_DepthTexture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        m_Width,
        m_Height,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_BORDER);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_BORDER);

    const float borderColor[] =
    {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    };

    glTexParameterfv(
        GL_TEXTURE_2D,
        GL_TEXTURE_BORDER_COLOR,
        borderColor);

    glBindFramebuffer(
        GL_FRAMEBUFFER,
        m_Framebuffer);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        m_DepthTexture,
        0);

    glDrawBuffer(
        GL_NONE);

    glReadBuffer(
        GL_NONE);

    bool complete =
        glCheckFramebufferStatus(
            GL_FRAMEBUFFER)
        ==
        GL_FRAMEBUFFER_COMPLETE;

    glBindFramebuffer(
        GL_FRAMEBUFFER,
        0);

    if (!complete)
    {
        std::cerr
            << "ERROR: ShadowMap framebuffer incompleto"
            << '\n';

        return false;
    }

    std::cout
        << "ShadowMap creada: "
        << width
        << "x"
        << height
        << '\n';

    return true;
}

void ShadowMap::BindForWriting() const
{
    glViewport(
        0,
        0,
        m_Width,
        m_Height);

    glBindFramebuffer(
        GL_FRAMEBUFFER,
        m_Framebuffer);

    glClear(
        GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::Unbind() const
{
    glBindFramebuffer(
        GL_FRAMEBUFFER,
        0);
}

void ShadowMap::BindTexture(
    uint32_t slot) const
{
    glActiveTexture(
        GL_TEXTURE0 + slot);

    glBindTexture(
        GL_TEXTURE_2D,
        m_DepthTexture);
}

uint32_t ShadowMap::GetWidth() const
{
    return m_Width;
}

uint32_t ShadowMap::GetHeight() const
{
    return m_Height;
}

}