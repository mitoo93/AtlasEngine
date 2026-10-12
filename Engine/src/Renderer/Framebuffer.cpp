#include "AtlasEngine/Renderer/Framebuffer.h"

#include <iostream>

namespace AtlasEngine
{

Framebuffer::Framebuffer(
    unsigned int width,
    unsigned int height,
    FramebufferColorFormat colorFormat)
    :
    m_Width(width),
    m_Height(height),
    m_ColorFormat(colorFormat)
{
    Invalidate();
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_RendererID);

    glDeleteTextures(1, &m_ColorAttachment);

    glDeleteRenderbuffers(1, &m_DepthAttachment);
}

void Framebuffer::Bind()
{
    glBindFramebuffer(
        GL_FRAMEBUFFER,
        m_RendererID);

    glViewport(
        0,
        0,
        m_Width,
        m_Height);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(
        GL_FRAMEBUFFER,
        0);
}

void Framebuffer::Resize(
    unsigned int width,
    unsigned int height)
{
    if (width == 0 || height == 0)
    {
        return;
    }

    if (width == m_Width && height == m_Height)
    {
        return;
    }

    m_Width = width;
    m_Height = height;

    Invalidate();
}

void Framebuffer::Invalidate()
{
    if (m_RendererID)
    {
        glDeleteFramebuffers(
            1,
            &m_RendererID);

        glDeleteTextures(
            1,
            &m_ColorAttachment);

        glDeleteRenderbuffers(
            1,
            &m_DepthAttachment);
    }

    glGenFramebuffers(
        1,
        &m_RendererID);

    glBindFramebuffer(
        GL_FRAMEBUFFER,
        m_RendererID);

    //------------------------------------------------
    // Color Texture
    //------------------------------------------------

    glGenTextures(
        1,
        &m_ColorAttachment);

    glBindTexture(
        GL_TEXTURE_2D,
        m_ColorAttachment);

    if (m_ColorFormat ==
        FramebufferColorFormat::RGBA16F)
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA16F,
            m_Width,
            m_Height,
            0,
            GL_RGBA,
            GL_FLOAT,
            nullptr);
    }
    else
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            m_Width,
            m_Height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr);
    }

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        m_ColorAttachment,
        0);

    //------------------------------------------------
    // Depth
    //------------------------------------------------

    glGenRenderbuffers(
        1,
        &m_DepthAttachment);

    glBindRenderbuffer(
        GL_RENDERBUFFER,
        m_DepthAttachment);

    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        m_Width,
        m_Height);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        m_DepthAttachment);

    //------------------------------------------------

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)
        != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout
            << "Framebuffer incompleto\n";
    }

    glBindFramebuffer(
        GL_FRAMEBUFFER,
        0);
}

}