#include "AtlasEngine/Renderer/BRDFLUT.h"

#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Mesh.h"

#include <glad/glad.h>

#include <iostream>


namespace AtlasEngine
{

BRDFLUT::~BRDFLUT()
{
    if (m_RendererID != 0)
    {
        glDeleteTextures(
            1,
            &m_RendererID);
    }
}


bool BRDFLUT::Generate(
    Shader& shader,
    Mesh& quad,
    unsigned int size)
{
    if (m_RendererID != 0)
    {
        glDeleteTextures(
            1,
            &m_RendererID);

        m_RendererID =
            0;
    }


    // ----------------------------------------------------
    // TEXTURE
    // ----------------------------------------------------

    glGenTextures(
        1,
        &m_RendererID);

    glBindTexture(
        GL_TEXTURE_2D,
        m_RendererID);


    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RG16F,
        size,
        size,
        0,
        GL_RG,
        GL_FLOAT,
        nullptr);


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);


    // ----------------------------------------------------
    // FRAMEBUFFER
    // ----------------------------------------------------

    unsigned int framebuffer = 0;

    glGenFramebuffers(
        1,
        &framebuffer);

    glBindFramebuffer(
        GL_FRAMEBUFFER,
        framebuffer);


    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        m_RendererID,
        0);


    if (glCheckFramebufferStatus(
            GL_FRAMEBUFFER) !=
        GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr
            << "ERROR: BRDF LUT framebuffer incompleto\n";

        glBindFramebuffer(
            GL_FRAMEBUFFER,
            0);

        glDeleteFramebuffers(
            1,
            &framebuffer);

        return false;
    }


    // ----------------------------------------------------
    // RENDER LUT
    // ----------------------------------------------------

    glViewport(
        0,
        0,
        size,
        size);


    glClear(
        GL_COLOR_BUFFER_BIT);


    shader.Bind();

    quad.Draw();


    // ----------------------------------------------------
    // CLEANUP
    // ----------------------------------------------------

    glBindFramebuffer(
        GL_FRAMEBUFFER,
        0);


    glDeleteFramebuffers(
        1,
        &framebuffer);


    glBindTexture(
        GL_TEXTURE_2D,
        0);


    return true;
}


void BRDFLUT::Bind(
    unsigned int slot) const
{
    glActiveTexture(
        GL_TEXTURE0 +
        slot);

    glBindTexture(
        GL_TEXTURE_2D,
        m_RendererID);
}

}