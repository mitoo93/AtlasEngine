#pragma once

#include <cstdint>

namespace AtlasEngine
{

struct RenderStats
{
    uint32_t DrawCalls = 0;

    uint32_t SceneDrawCalls = 0;
    uint32_t ShadowDrawCalls = 0;
    uint32_t PostProcessDrawCalls = 0;

    uint64_t Vertices = 0;
    uint64_t Indices = 0;
    uint64_t Triangles = 0;

    uint32_t RenderedObjects = 0;

    uint32_t SkyboxDrawCalls = 0;

    uint32_t SubmittedObjects = 0;
    uint32_t CulledObjects = 0;

    uint32_t SceneShaderBinds = 0;
    uint32_t SceneMaterialBinds = 0;

    void Reset()
    {
        DrawCalls = 0;

        SceneDrawCalls = 0;
        ShadowDrawCalls = 0;
        PostProcessDrawCalls = 0;

        Vertices = 0;
        Indices = 0;
        Triangles = 0;

        RenderedObjects = 0;

        SkyboxDrawCalls = 0;

        SubmittedObjects = 0;

        CulledObjects = 0;

        SceneShaderBinds = 0;
        SceneMaterialBinds = 0;
    }
};

}