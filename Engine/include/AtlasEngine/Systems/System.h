#pragma once

namespace AtlasEngine
{

class Scene;

class System
{
public:

    virtual ~System() = default;

    virtual void Update(
        Scene& scene) = 0;
};

} // namespace AtlasEngine