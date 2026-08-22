#pragma once

#include <memory>

namespace AtlasEngine
{

class Mesh;

class MeshFactory
{
public:

    static std::unique_ptr<Mesh> CreateQuad();

    static std::unique_ptr<Mesh> CreateSphere(
        float radius = 1.0f,
        unsigned int segments = 32,
        unsigned int rings = 16);

    static std::unique_ptr<Mesh>
        CreateSkyboxCube();
};

}