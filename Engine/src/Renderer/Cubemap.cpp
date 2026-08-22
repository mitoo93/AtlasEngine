#include "AtlasEngine/Renderer/Cubemap.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Math/Matrix4.h"
#include "AtlasEngine/Math/Vector3.h"

#include <glad/glad.h>

#include "stb_image.h"

#include <iostream>
#include <algorithm>


namespace AtlasEngine
{

Cubemap::Cubemap()
{
}


Cubemap::~Cubemap()
{
    if (m_RendererID != 0)
    {
        glDeleteTextures(
            1,
            &m_RendererID);
    }
}


bool Cubemap::Load(
    const std::vector<std::string>& faces)
{
    // Un cubemap necesita exactamente
    // las seis caras del cubo.

    if (faces.size() != 6)
    {
        std::cerr
            << "ERROR: Cubemap necesita exactamente 6 caras"
            << std::endl;

        return false;
    }


    // Si por algún motivo intentamos recargar
    // este mismo Cubemap, eliminamos primero
    // la textura OpenGL anterior.

    if (m_RendererID != 0)
    {
        glDeleteTextures(
            1,
            &m_RendererID);

        m_RendererID = 0;
    }


    glGenTextures(
        1,
        &m_RendererID);

    glBindTexture(
        GL_TEXTURE_CUBE_MAP,
        m_RendererID);


    // Para cubemaps NO queremos el mismo flip
    // vertical que usamos normalmente para
    // texturas 2D.

    stbi_set_flip_vertically_on_load(
        false);


    for (unsigned int i = 0;
         i < faces.size();
         ++i)
    {
        int width = 0;
        int height = 0;
        int channels = 0;


        unsigned char* data =
            stbi_load(
                faces[i].c_str(),
                &width,
                &height,
                &channels,
                0);


        if (!data)
        {
            std::cerr
                << "ERROR cargando cara del Cubemap: "
                << faces[i]
                << std::endl;


            glBindTexture(
                GL_TEXTURE_CUBE_MAP,
                0);

            glDeleteTextures(
                1,
                &m_RendererID);

            m_RendererID = 0;

            return false;
        }


        GLenum format =
            GL_RGB;

        if (channels == 1)
        {
            format =
                GL_RED;
        }
        else if (channels == 3)
        {
            format =
                GL_RGB;
        }
        else if (channels == 4)
        {
            format =
                GL_RGBA;
        }
        else
        {
            std::cerr
                << "ERROR: numero de canales no soportado en Cubemap: "
                << channels
                << " | "
                << faces[i]
                << std::endl;

            stbi_image_free(
                data);

            glBindTexture(
                GL_TEXTURE_CUBE_MAP,
                0);

            glDeleteTextures(
                1,
                &m_RendererID);

            m_RendererID = 0;

            return false;
        }


        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data);


        stbi_image_free(
            data);
    }

    glGenerateMipmap(
        GL_TEXTURE_CUBE_MAP);

    // ------------------------------------------------
    // FILTERING
    // ------------------------------------------------

    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);


    // ------------------------------------------------
    // WRAPPING
    // ------------------------------------------------
    //
    // CLAMP_TO_EDGE es especialmente importante
    // para evitar seams entre las caras.

    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_WRAP_R,
        GL_CLAMP_TO_EDGE);


    glBindTexture(
        GL_TEXTURE_CUBE_MAP,
        0);


    std::cout
        << "Cubemap cargado correctamente"
        << std::endl;


    return true;
}




void Cubemap::Bind(
    unsigned int slot) const
{
    glActiveTexture(
        GL_TEXTURE0 + slot);

    glBindTexture(
        GL_TEXTURE_CUBE_MAP,
        m_RendererID);
}


void Cubemap::Unbind() const
{
    glBindTexture(
        GL_TEXTURE_CUBE_MAP,
        0);
}


void Cubemap::SetName(
    const std::string& name)
{
    m_Name =
        name;
}


const std::string& Cubemap::GetName() const
{
    return m_Name;
}

bool Cubemap::Create(
    unsigned int size,
    bool generateMipmaps)
{
    if (m_RendererID != 0)
    {
        glDeleteTextures(
            1,
            &m_RendererID);

        m_RendererID =
            0;
    }

    glGenTextures(
        1,
        &m_RendererID);

    glBindTexture(
        GL_TEXTURE_CUBE_MAP,
        m_RendererID);


    for (unsigned int i = 0;
         i < 6;
         ++i)
    {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            GL_RGB16F,
            size,
            size,
            0,
            GL_RGB,
            GL_FLOAT,
            nullptr);
    }


    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_WRAP_R,
        GL_CLAMP_TO_EDGE);


    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);


    if (generateMipmaps)
    {
        glTexParameteri(
            GL_TEXTURE_CUBE_MAP,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);

        glGenerateMipmap(
            GL_TEXTURE_CUBE_MAP);
    }
    else
    {
        glTexParameteri(
            GL_TEXTURE_CUBE_MAP,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR);
    }


    glBindTexture(
        GL_TEXTURE_CUBE_MAP,
        0);

    return true;
}

bool Cubemap::GenerateIrradianceMap(
    Cubemap& environmentMap,
    Shader& irradianceShader,
    Mesh& cubeMesh,
    unsigned int size)
{
    // ----------------------------------------------------
    // CREATE IRRADIANCE CUBEMAP
    // ----------------------------------------------------

    if (!Create(
            size,
            false))
    {
        return false;
    }


    // ----------------------------------------------------
    // CAPTURE FRAMEBUFFER
    // ----------------------------------------------------

    unsigned int captureFramebuffer = 0;
    unsigned int captureRenderbuffer = 0;


    glGenFramebuffers(
        1,
        &captureFramebuffer);

    glGenRenderbuffers(
        1,
        &captureRenderbuffer);


    glBindFramebuffer(
        GL_FRAMEBUFFER,
        captureFramebuffer);


    glBindRenderbuffer(
        GL_RENDERBUFFER,
        captureRenderbuffer);

    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH_COMPONENT24,
        size,
        size);


    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        captureRenderbuffer);


    // ----------------------------------------------------
    // CAPTURE PROJECTION
    // ----------------------------------------------------

    Matrix4 captureProjection =
        Matrix4::Perspective(
            90.0f,
            1.0f,
            0.1f,
            10.0f);


    // ----------------------------------------------------
    // CAPTURE VIEWS
    // ----------------------------------------------------

    Matrix4 captureViews[6] =
    {
        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(1.0f, 0.0f, 0.0f),
            Vector3(0.0f, -1.0f, 0.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(-1.0f, 0.0f, 0.0f),
            Vector3(0.0f, -1.0f, 0.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, 1.0f, 0.0f),
            Vector3(0.0f, 0.0f, 1.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, -1.0f, 0.0f),
            Vector3(0.0f, 0.0f, -1.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, 0.0f, 1.0f),
            Vector3(0.0f, -1.0f, 0.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, 0.0f, -1.0f),
            Vector3(0.0f, -1.0f, 0.0f))
    };


    // ----------------------------------------------------
    // IRRADIANCE SHADER
    // ----------------------------------------------------

    irradianceShader.Bind();

    irradianceShader.SetInt(
        "u_EnvironmentMap",
        0);

    irradianceShader.SetMatrix4(
        "u_Projection",
        captureProjection);


    environmentMap.Bind(
        0);


    // ----------------------------------------------------
    // RENDER EACH CUBEMAP FACE
    // ----------------------------------------------------

    glViewport(
        0,
        0,
        size,
        size);


    glBindFramebuffer(
        GL_FRAMEBUFFER,
        captureFramebuffer);


    for (unsigned int i = 0;
         i < 6;
         ++i)
    {
        irradianceShader.SetMatrix4(
            "u_View",
            captureViews[i]);


        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            m_RendererID,
            0);


        GLenum framebufferStatus =
            glCheckFramebufferStatus(
                GL_FRAMEBUFFER);

        if (framebufferStatus !=
            GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr
                << "ERROR: Irradiance framebuffer incompleto\n";

            glBindFramebuffer(
                GL_FRAMEBUFFER,
                0);

            glDeleteRenderbuffers(
                1,
                &captureRenderbuffer);

            glDeleteFramebuffers(
                1,
                &captureFramebuffer);

            return false;
        }


        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);


        cubeMesh.Draw();
    }


    // ----------------------------------------------------
    // CLEANUP
    // ----------------------------------------------------

    glBindFramebuffer(
        GL_FRAMEBUFFER,
        0);


    glDeleteRenderbuffers(
        1,
        &captureRenderbuffer);

    glDeleteFramebuffers(
        1,
        &captureFramebuffer);


    return true;
}

bool Cubemap::GeneratePrefilterMap(
    Cubemap& environmentMap,
    Shader& prefilterShader,
    Mesh& cubeMesh,
    unsigned int baseSize,
    unsigned int mipLevels)
{
    if (!Create(
            baseSize,
            true))
    {
        return false;
    }

    unsigned int captureFramebuffer = 0;
    unsigned int captureRenderbuffer = 0;

    glGenFramebuffers(
        1,
        &captureFramebuffer);

    glGenRenderbuffers(
        1,
        &captureRenderbuffer);

    glBindFramebuffer(
        GL_FRAMEBUFFER,
        captureFramebuffer);

    glBindRenderbuffer(
        GL_RENDERBUFFER,
        captureRenderbuffer);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        captureRenderbuffer);


    Matrix4 captureProjection =
        Matrix4::Perspective(
            90.0f,
            1.0f,
            0.1f,
            10.0f);


    Matrix4 captureViews[6] =
    {
        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(1.0f, 0.0f, 0.0f),
            Vector3(0.0f, -1.0f, 0.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(-1.0f, 0.0f, 0.0f),
            Vector3(0.0f, -1.0f, 0.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, 1.0f, 0.0f),
            Vector3(0.0f, 0.0f, 1.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, -1.0f, 0.0f),
            Vector3(0.0f, 0.0f, -1.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, 0.0f, 1.0f),
            Vector3(0.0f, -1.0f, 0.0f)),

        Matrix4::LookAt(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, 0.0f, -1.0f),
            Vector3(0.0f, -1.0f, 0.0f))
    };


    prefilterShader.Bind();

    prefilterShader.SetInt(
        "u_EnvironmentMap",
        0);

    prefilterShader.SetMatrix4(
        "u_Projection",
        captureProjection);

    environmentMap.Bind(
        0);


    for (unsigned int mip = 0;
         mip < mipLevels;
         ++mip)
    {
        unsigned int mipWidth =
            std::max(
                1u,
                baseSize >> mip);

        unsigned int mipHeight =
            std::max(
                1u,
                baseSize >> mip);


        glBindRenderbuffer(
            GL_RENDERBUFFER,
            captureRenderbuffer);

        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT24,
            mipWidth,
            mipHeight);


        glViewport(
            0,
            0,
            mipWidth,
            mipHeight);


        float roughness =
            static_cast<float>(mip) /
            static_cast<float>(
                mipLevels - 1);


        prefilterShader.SetFloat(
            "u_Roughness",
            roughness);


        for (unsigned int face = 0;
             face < 6;
             ++face)
        {
            prefilterShader.SetMatrix4(
                "u_View",
                captureViews[face]);


            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                m_RendererID,
                mip);


            GLenum framebufferStatus =
                glCheckFramebufferStatus(
                    GL_FRAMEBUFFER);

            if (framebufferStatus !=
                GL_FRAMEBUFFER_COMPLETE)
            {
                std::cerr
                    << "ERROR: Prefilter framebuffer incompleto\n";

                glBindFramebuffer(
                    GL_FRAMEBUFFER,
                    0);

                glDeleteRenderbuffers(
                    1,
                    &captureRenderbuffer);

                glDeleteFramebuffers(
                    1,
                    &captureFramebuffer);

                return false;
            }


            glClear(
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);


            cubeMesh.Draw();
        }
    }


    glBindFramebuffer(
        GL_FRAMEBUFFER,
        0);


    glDeleteRenderbuffers(
        1,
        &captureRenderbuffer);

    glDeleteFramebuffers(
        1,
        &captureFramebuffer);


    return true;
}

}