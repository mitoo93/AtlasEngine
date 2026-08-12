#include "AtlasEngine/Renderer/Renderer.h"
#include "AtlasEngine/Renderer/RenderCommand.h"

#include <iostream>

namespace AtlasEngine
{

std::unique_ptr<Framebuffer> Renderer::s_Framebuffer;

void Renderer::InitFramebuffer(
    uint32_t width,
    uint32_t height)
{
    s_Framebuffer =
        std::make_unique<Framebuffer>(
            width,
            height);
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

void Renderer::BeginFrame()
{
    s_Framebuffer->Bind();

    glViewport(
        0,
        0,
        s_Framebuffer->GetWidth(),
        s_Framebuffer->GetHeight());

    RenderCommand::SetClearColor(
        0.15f,
        0.15f,
        0.18f,
        1.0f);

    RenderCommand::Clear();
}

void Renderer::EndFrame()
{
    s_Framebuffer->Unbind();
}

void Renderer::ResizeFramebuffer(
    uint32_t width,
    uint32_t height)
{
    if (!s_Framebuffer)
        return;

    if (s_Framebuffer->GetWidth() == width &&
        s_Framebuffer->GetHeight() == height)
    {
        return;
    }

    s_Framebuffer->Resize(
        width,
        height);
}

}