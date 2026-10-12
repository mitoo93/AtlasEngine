#pragma once

#include <memory>
#include <cstdint>

#include "AtlasEngine/Renderer/Framebuffer.h"
#include "AtlasEngine/Renderer/RenderStats.h"

namespace AtlasEngine
{

class Scene;
class Shader;
class BRDFLUT;

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

    static bool InitBRDFLUT();

    static Framebuffer& GetFramebuffer();
    static Framebuffer& GetHDRFramebuffer();

    static void ResizeFramebuffer(
        uint32_t width,
        uint32_t height);

    static void SetExposure(
        float exposure);

    static float GetExposure();

    static void SetGrayscaleEnabled(
        bool enabled);

    static bool IsGrayscaleEnabled();


    static void SetVignetteEnabled(
        bool enabled);

    static bool IsVignetteEnabled();


    static void SetVignetteStrength(
        float strength);

    static float GetVignetteStrength();

    static void SetBloomEnabled(
        bool enabled);

    static bool IsBloomEnabled();

    static void SetBloomStrength(
        float strength);

    static float GetBloomStrength();

    static void SetBloomThreshold(
        float threshold);

    static float GetBloomThreshold();

    static BRDFLUT* GetBRDFLUT();

    static const RenderStats& GetStats();

    static void ResetStats();

    static void SubmitSceneDraw(
        uint32_t vertexCount,
        uint32_t indexCount,
        uint32_t instanceCount = 1);

    static void SubmitShadowDraw(
        uint32_t vertexCount,
        uint32_t indexCount,
        uint32_t instanceCount = 1);

    static void SubmitPostProcessDraw();

    static void SubmitSkyboxDraw();

    static void SetBloomBlurPasses(
        int passes);

    static int GetBloomBlurPasses();

    static void SubmitObject();

    static void SubmitCulledObject();

    static void SubmitSceneShaderBind();
    static void SubmitSceneMaterialBind();

private:

    static std::unique_ptr<Framebuffer> s_Framebuffer;
    static std::unique_ptr<Framebuffer> s_HDRFramebuffer;
    static std::unique_ptr<Framebuffer> s_BrightFramebuffer;
    static std::unique_ptr<Framebuffer> s_BlurFramebufferA;
    static std::unique_ptr<Framebuffer> s_BlurFramebufferB;
    static std::unique_ptr<Framebuffer> s_BloomFramebuffer;
    static std::unique_ptr<BRDFLUT> s_BRDFLUT;

    static float s_Exposure;
    
    static bool s_GrayscaleEnabled;

    static bool s_VignetteEnabled;

    static float s_VignetteStrength;

    static bool s_BloomEnabled;

    static float s_BloomStrength;

    static float s_BloomThreshold;

    static RenderStats s_RenderStats;

    static int s_BloomBlurPasses;
};

}