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
    return *s_Framebuffer;
}

void Renderer::BeginFrame()
{
    s_Framebuffer->Bind();

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

}