#pragma once

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
};

}