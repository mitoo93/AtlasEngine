#pragma once

#include <string>

namespace AtlasEngine
{

class Scene;

class SceneSerializer
{
public:

    static bool Save(
        const Scene& scene,
        const std::string& filepath);

    static bool Load(
        Scene& scene,
        const std::string& filepath);
};

}