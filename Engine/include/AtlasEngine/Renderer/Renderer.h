#pragma once

#include <memory>
#include <cstdint>

#include "AtlasEngine/Renderer/Framebuffer.h"

namespace AtlasEngine
{

class Scene;
class Shader;

class Renderer
{
public:

    static void BeginFrame();

    static void EndFrame();

    static void DrawScene(
        const Scene& scene,
        Shader& shader);

    static void InitFramebuffer(
    uint32_t width,
    uint32_t height);

    static Framebuffer& GetFramebuffer();

    static void ResizeFramebuffer(
        uint32_t width,
        uint32_t height);

private:

    static std::unique_ptr<Framebuffer> s_Framebuffer;
};

}