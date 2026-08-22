#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Renderer/RenderCommand.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/BRDFLUT.h"

#include "AtlasEngine/Assets/AssetManager.h"

#include <iostream>
#include <algorithm>

namespace AtlasEngine
{

std::unique_ptr<Framebuffer> Renderer::s_Framebuffer;
std::unique_ptr<Framebuffer> Renderer::s_HDRFramebuffer;
std::unique_ptr<Framebuffer> Renderer::s_BrightFramebuffer;
std::unique_ptr<Framebuffer> Renderer::s_BlurFramebufferA;
std::unique_ptr<Framebuffer> Renderer::s_BlurFramebufferB;
std::unique_ptr<Framebuffer> Renderer::s_BloomFramebuffer;
std::unique_ptr<BRDFLUT> Renderer::s_BRDFLUT;

float Renderer::s_Exposure = 1.0f;

bool Renderer::s_GrayscaleEnabled =
    false;

bool Renderer::s_VignetteEnabled =
    false;

float Renderer::s_VignetteStrength =
    1.5f;

bool Renderer::s_BloomEnabled =
    true;

float Renderer::s_BloomStrength =
    0.2f;

float Renderer::s_BloomThreshold =
    1.0f;

void Renderer::InitFramebuffer(
    uint32_t width,
    uint32_t height)
{
    const uint32_t bloomWidth =
        std::max(
            1u,
            width / 2);

    const uint32_t bloomHeight =
        std::max(
            1u,
            height / 2);


    // ====================================================
    // FULL RESOLUTION
    // ====================================================

    s_Framebuffer =
        std::make_unique<Framebuffer>(
            width,
            height,
            FramebufferColorFormat::RGBA8);

    s_HDRFramebuffer =
        std::make_unique<Framebuffer>(
            width,
            height,
            FramebufferColorFormat::RGBA16F);


    // ====================================================
    // BLOOM - HALF RESOLUTION
    // ====================================================

    s_BrightFramebuffer =
        std::make_unique<Framebuffer>(
            bloomWidth,
            bloomHeight,
            FramebufferColorFormat::RGBA16F);

    s_BlurFramebufferA =
        std::make_unique<Framebuffer>(
            bloomWidth,
            bloomHeight,
            FramebufferColorFormat::RGBA16F);

    s_BlurFramebufferB =
        std::make_unique<Framebuffer>(
            bloomWidth,
            bloomHeight,
            FramebufferColorFormat::RGBA16F);


    // ====================================================
    // BLOOM COMPOSITE - FULL RESOLUTION
    // ====================================================

    s_BloomFramebuffer =
        std::make_unique<Framebuffer>(
            width,
            height,
            FramebufferColorFormat::RGBA16F);
}

Framebuffer& Renderer::GetFramebuffer()
{
    if (!s_Framebuffer)
    {
        std::cerr
            << "ERROR: Renderer::GetFramebuffer() llamado "
               "antes de InitFramebuffer()\n";

        std::abort();
    }

    return *s_Framebuffer;
}

Framebuffer& Renderer::GetHDRFramebuffer()
{
    if (!s_HDRFramebuffer)
    {
        std::cerr
            << "ERROR: Renderer::GetHDRFramebuffer() llamado "
               "antes de InitFramebuffer()\n";

        std::abort();
    }

    return *s_HDRFramebuffer;
}

void Renderer::BeginFrame()
{
    s_HDRFramebuffer->Bind();

    glViewport(
        0,
        0,
        s_HDRFramebuffer->GetWidth(),
        s_HDRFramebuffer->GetHeight());

    RenderCommand::SetClearColor(
        0.15f,
        0.15f,
        0.18f,
        1.0f);

    RenderCommand::Clear();
}

void Renderer::EndFrame()
{
    Shader* brightPassShader =
        AssetManager::GetShader(
            "BrightPass");

    Shader* blurShader =
        AssetManager::GetShader(
            "GaussianBlur");

    Shader* bloomShader =
        AssetManager::GetShader(
            "BloomComposite");

    Shader* toneMappingShader =
        AssetManager::GetShader(
            "ToneMapping");

    Mesh* fullscreenQuad =
        AssetManager::GetMesh(
            "Quad");


    if (!brightPassShader ||
        !blurShader ||
        !bloomShader ||
        !toneMappingShader ||
        !fullscreenQuad)
    {
        s_HDRFramebuffer->Unbind();
        return;
    }


    glDisable(
        GL_DEPTH_TEST);

    // ====================================================
    // BLOOM
    // ====================================================

    if (s_BloomEnabled)
    {

        // ====================================================
        // 1. BRIGHT PASS
        // HDR -> BRIGHT
        // ====================================================

        s_BrightFramebuffer->Bind();

        glViewport(
            0,
            0,
            s_BrightFramebuffer->GetWidth(),
            s_BrightFramebuffer->GetHeight());


        RenderCommand::SetClearColor(
            0.0f,
            0.0f,
            0.0f,
            1.0f);

        RenderCommand::Clear();


        brightPassShader->Bind();


        glActiveTexture(
            GL_TEXTURE0);

        glBindTexture(
            GL_TEXTURE_2D,
            s_HDRFramebuffer
                ->GetColorAttachment());


        brightPassShader->SetInt(
            "u_HDRTexture",
            0);

        brightPassShader->SetFloat(
            "u_Threshold",
            s_BloomThreshold);


        fullscreenQuad->Draw();


        // ====================================================
        // 2. GAUSSIAN BLUR
        // ====================================================

        blurShader->Bind();

        blurShader->SetInt(
            "u_Image",
            0);


        Framebuffer* finalBlurFramebuffer =
            nullptr;

        bool horizontal =
            true;

        bool firstIteration =
            true;

        const int blurPasses =
            10;


        for (int i = 0;
            i < blurPasses;
            ++i)
        {
            Framebuffer* target =
                horizontal
                    ? s_BlurFramebufferA.get()
                    : s_BlurFramebufferB.get();


            target->Bind();


            glViewport(
                0,
                0,
                target->GetWidth(),
                target->GetHeight());


            RenderCommand::SetClearColor(
                0.0f,
                0.0f,
                0.0f,
                1.0f);

            RenderCommand::Clear();


            blurShader->Bind();

            blurShader->SetBool(
                "u_Horizontal",
                horizontal);


            glActiveTexture(
                GL_TEXTURE0);


            if (firstIteration)
            {
                glBindTexture(
                    GL_TEXTURE_2D,
                    s_BrightFramebuffer
                        ->GetColorAttachment());
            }
            else
            {
                Framebuffer* source =
                    horizontal
                        ? s_BlurFramebufferB.get()
                        : s_BlurFramebufferA.get();


                glBindTexture(
                    GL_TEXTURE_2D,
                    source
                        ->GetColorAttachment());
            }


            fullscreenQuad->Draw();

            finalBlurFramebuffer =
                target;

            horizontal =
                !horizontal;

            firstIteration =
                false;
        }


        // ====================================================
        // 3. BLOOM COMPOSITE
        //
        // HDR + BLURRED BRIGHT -> BLOOM HDR
        // ====================================================

        s_BloomFramebuffer->Bind();


        glViewport(
            0,
            0,
            s_BloomFramebuffer->GetWidth(),
            s_BloomFramebuffer->GetHeight());


        RenderCommand::SetClearColor(
            0.0f,
            0.0f,
            0.0f,
            1.0f);

        RenderCommand::Clear();


        bloomShader->Bind();


        // Original HDR
        glActiveTexture(
            GL_TEXTURE0);

        glBindTexture(
            GL_TEXTURE_2D,
            s_HDRFramebuffer
                ->GetColorAttachment());

        bloomShader->SetInt(
            "u_HDRTexture",
            0);


        // Blurred Bright Pass
        glActiveTexture(
            GL_TEXTURE1);

        glBindTexture(
            GL_TEXTURE_2D,
            finalBlurFramebuffer
                ->GetColorAttachment());

        bloomShader->SetInt(
            "u_BloomTexture",
            1);


        bloomShader->SetFloat(
            "u_BloomStrength",
            s_BloomStrength);


        fullscreenQuad->Draw();
    }


    // ====================================================
    // 4. TONE MAPPING
    //
    // BLOOM HDR -> FINAL RGBA8
    // ====================================================

    s_Framebuffer->Bind();


    glViewport(
        0,
        0,
        s_Framebuffer->GetWidth(),
        s_Framebuffer->GetHeight());


    RenderCommand::SetClearColor(
        0.0f,
        0.0f,
        0.0f,
        1.0f);

    RenderCommand::Clear();


    toneMappingShader->Bind();


    glActiveTexture(
        GL_TEXTURE0);

    glBindTexture(
        GL_TEXTURE_2D,
        s_BloomEnabled
            ? s_BloomFramebuffer
                ->GetColorAttachment()
            : s_HDRFramebuffer
                ->GetColorAttachment());


    toneMappingShader->SetInt(
        "u_HDRTexture",
        0);


    toneMappingShader->SetFloat(
        "u_Exposure",
        s_Exposure);


    toneMappingShader->SetBool(
        "u_EnableGrayscale",
        s_GrayscaleEnabled);


    toneMappingShader->SetBool(
        "u_EnableVignette",
        s_VignetteEnabled);


    toneMappingShader->SetFloat(
        "u_VignetteStrength",
        s_VignetteStrength);


    fullscreenQuad->Draw();


    // ====================================================
    // CLEANUP
    // ====================================================

    glActiveTexture(
        GL_TEXTURE1);

    glBindTexture(
        GL_TEXTURE_2D,
        0);


    glActiveTexture(
        GL_TEXTURE0);

    glBindTexture(
        GL_TEXTURE_2D,
        0);


    glEnable(
        GL_DEPTH_TEST);


    s_Framebuffer->Unbind();
}

void Renderer::ResizeFramebuffer(
    uint32_t width,
    uint32_t height)
{
    if (!s_Framebuffer ||
        !s_HDRFramebuffer ||
        !s_BrightFramebuffer ||
        !s_BlurFramebufferA ||
        !s_BlurFramebufferB ||
        !s_BloomFramebuffer)
    {
        return;
    }


    const uint32_t bloomWidth =
        std::max(
            1u,
            width / 2);

    const uint32_t bloomHeight =
        std::max(
            1u,
            height / 2);


    // ====================================================
    // FINAL FRAMEBUFFER - FULL RESOLUTION
    // ====================================================

    if (s_Framebuffer->GetWidth() != width ||
        s_Framebuffer->GetHeight() != height)
    {
        s_Framebuffer->Resize(
            width,
            height);
    }


    // ====================================================
    // HDR FRAMEBUFFER - FULL RESOLUTION
    // ====================================================

    if (s_HDRFramebuffer->GetWidth() != width ||
        s_HDRFramebuffer->GetHeight() != height)
    {
        s_HDRFramebuffer->Resize(
            width,
            height);
    }


    // ====================================================
    // BRIGHT PASS - HALF RESOLUTION
    // ====================================================

    if (s_BrightFramebuffer->GetWidth() != bloomWidth ||
        s_BrightFramebuffer->GetHeight() != bloomHeight)
    {
        s_BrightFramebuffer->Resize(
            bloomWidth,
            bloomHeight);
    }


    // ====================================================
    // BLUR A - HALF RESOLUTION
    // ====================================================

    if (s_BlurFramebufferA->GetWidth() != bloomWidth ||
        s_BlurFramebufferA->GetHeight() != bloomHeight)
    {
        s_BlurFramebufferA->Resize(
            bloomWidth,
            bloomHeight);
    }


    // ====================================================
    // BLUR B - HALF RESOLUTION
    // ====================================================

    if (s_BlurFramebufferB->GetWidth() != bloomWidth ||
        s_BlurFramebufferB->GetHeight() != bloomHeight)
    {
        s_BlurFramebufferB->Resize(
            bloomWidth,
            bloomHeight);
    }


    // ====================================================
    // BLOOM COMPOSITE - FULL RESOLUTION
    // ====================================================

    if (s_BloomFramebuffer->GetWidth() != width ||
        s_BloomFramebuffer->GetHeight() != height)
    {
        s_BloomFramebuffer->Resize(
            width,
            height);
    }
}

void Renderer::SetExposure(
    float exposure)
{
    s_Exposure =
        exposure;
}


float Renderer::GetExposure()
{
    return s_Exposure;
}

void Renderer::SetGrayscaleEnabled(
    bool enabled)
{
    s_GrayscaleEnabled =
        enabled;
}


bool Renderer::IsGrayscaleEnabled()
{
    return s_GrayscaleEnabled;
}


void Renderer::SetVignetteEnabled(
    bool enabled)
{
    s_VignetteEnabled =
        enabled;
}


bool Renderer::IsVignetteEnabled()
{
    return s_VignetteEnabled;
}


void Renderer::SetVignetteStrength(
    float strength)
{
    s_VignetteStrength =
        strength;
}


float Renderer::GetVignetteStrength()
{
    return s_VignetteStrength;
}

void Renderer::SetBloomEnabled(
    bool enabled)
{
    s_BloomEnabled =
        enabled;
}

bool Renderer::IsBloomEnabled()
{
    return s_BloomEnabled;
}

void Renderer::SetBloomStrength(
    float strength)
{
    s_BloomStrength =
        strength;
}

float Renderer::GetBloomStrength()
{
    return s_BloomStrength;
}

void Renderer::SetBloomThreshold(
    float threshold)
{
    s_BloomThreshold =
        threshold;
}

float Renderer::GetBloomThreshold()
{
    return s_BloomThreshold;
}

bool Renderer::InitBRDFLUT()
{
    Shader* brdfShader =
        AssetManager::GetShader(
            "BRDF");

    Mesh* quad =
        AssetManager::GetMesh(
            "Quad");

    if (!brdfShader ||
        !quad)
    {
        std::cerr
            << "ERROR: faltan recursos para generar BRDF LUT\n";

        return false;
    }

    s_BRDFLUT =
        std::make_unique<BRDFLUT>();

    if (!s_BRDFLUT->Generate(
            *brdfShader,
            *quad,
            512))
    {
        std::cerr
            << "ERROR generando BRDF LUT\n";

        s_BRDFLUT.reset();

        return false;
    }

    std::cout
        << "BRDF LUT generada correctamente\n";

    return true;
}

BRDFLUT* Renderer::GetBRDFLUT()
{
    return
        s_BRDFLUT.get();
}

}