#pragma once

#include "AtlasEngine/Math/Matrix4.h"

namespace AtlasEngine
{

class Mesh;
class Material;
class Shader;

struct RenderItem
{
    Mesh* MeshPtr =
        nullptr;

    Material* MaterialPtr =
        nullptr;

    Shader* ShaderPtr =
        nullptr;

    Matrix4 ModelMatrix;
};

}