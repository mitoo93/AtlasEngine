#pragma once

#include "AtlasEngine/Systems/System.h"

namespace AtlasEngine
{

class Shader;

class RenderSystem : public System
{
public:

    void Update(
        Scene& scene) override;
};

}