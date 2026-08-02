#pragma once

#include <memory>

namespace AtlasEngine
{

class Mesh;

class MeshFactory
{
public:

    static std::unique_ptr<Mesh> CreateQuad();
};

}