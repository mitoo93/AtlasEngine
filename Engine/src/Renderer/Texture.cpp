#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "AtlasEngine/Renderer/Texture.h"

namespace AtlasEngine
{

namespace
{

bool CreateTextureFromPixels(
    unsigned int& rendererID,
    int width,
    int height,
    int channels,
    const unsigned char* data)
{
    if (!data)
        return false;

    GLenum format =
        GL_RGB;

    if (channels == 4)
    {
        format =
            GL_RGBA;
    }
    else if (channels == 1)
    {
        format =
            GL_RED;
    }

    glGenTextures(
        1,
        &rendererID);

    glBindTexture(
        GL_TEXTURE_2D,
        rendererID);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data);

    glGenerateMipmap(
        GL_TEXTURE_2D);

    glBindTexture(
        GL_TEXTURE_2D,
        0);

    return true;
}

}


// ----------------------------------------------------
// CONSTRUCTOR
// ----------------------------------------------------

Texture::Texture()
{
}


// ----------------------------------------------------
// DESTRUCTOR
// ----------------------------------------------------

Texture::~Texture()
{
    if (m_RendererID != 0)
    {
        glDeleteTextures(
            1,
            &m_RendererID);
    }
}


// ----------------------------------------------------
// LOAD FROM FILE
// ----------------------------------------------------

bool Texture::Load(
    const std::string& path)
{
    stbi_set_flip_vertically_on_load(
        true);

    unsigned char* data =
        stbi_load(
            path.c_str(),
            &m_Width,
            &m_Height,
            &m_Channels,
            0);

    if (!data)
    {
        return false;
    }

    bool result =
        CreateTextureFromPixels(
            m_RendererID,
            m_Width,
            m_Height,
            m_Channels,
            data);

    stbi_image_free(
        data);

    return result;
}


// ----------------------------------------------------
// LOAD FROM MEMORY
// ----------------------------------------------------

bool Texture::LoadFromMemory(
    const unsigned char* data,
    int size)
{
    if (!data ||
        size <= 0)
    {
        return false;
    }

    stbi_set_flip_vertically_on_load(
        true);

    unsigned char* pixels =
        stbi_load_from_memory(
            data,
            size,
            &m_Width,
            &m_Height,
            &m_Channels,
            0);

    if (!pixels)
    {
        return false;
    }

    bool result =
        CreateTextureFromPixels(
            m_RendererID,
            m_Width,
            m_Height,
            m_Channels,
            pixels);

    stbi_image_free(
        pixels);

    return result;
}


// ----------------------------------------------------
// BIND
// ----------------------------------------------------

void Texture::Bind(
    unsigned int slot) const
{
    glActiveTexture(
        GL_TEXTURE0 + slot);

    glBindTexture(
        GL_TEXTURE_2D,
        m_RendererID);
}


void Texture::Unbind() const
{
    glBindTexture(
        GL_TEXTURE_2D,
        0);
}


// ----------------------------------------------------
// GETTERS
// ----------------------------------------------------

int Texture::GetWidth() const
{
    return m_Width;
}


int Texture::GetHeight() const
{
    return m_Height;
}


void Texture::SetName(
    const std::string& name)
{
    m_Name =
        name;
}


const std::string& Texture::GetName() const
{
    return m_Name;
}

}