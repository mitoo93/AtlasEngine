#pragma once

namespace AtlasEngine
{

class Scene;
class Shader;

class System
{
public:

    virtual ~System() = default;

    virtual void Update(
        Scene& scene,
        Shader& shader)
    {
    }
};

}