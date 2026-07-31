#pragma once

#include <memory>
#include <vector>

#include "AtlasEngine/Scene/Entity.h"

namespace AtlasEngine
{

class Scene
{
public:

    Entity& CreateEntity(
        const std::string& name);

    std::size_t GetEntityCount() const;

    Entity& GetEntity(
        std::size_t index);

    const Entity& GetEntity(
        std::size_t index) const;

private:

    EntityID m_NextEntityID = 1;
    std::vector<std::unique_ptr<Entity>> m_Entities;
};

} // namespace AtlasEngine