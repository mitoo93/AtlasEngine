#include "AtlasEngine/Scene/Scene.h"

#include <iostream>

namespace AtlasEngine
{

Entity& Scene::CreateEntity(
    const std::string& name)
{
    m_Entities.push_back(
        std::make_unique<Entity>(
            m_NextEntityID++,
            name));

    return *m_Entities.back();

}

std::size_t Scene::GetEntityCount() const
{
    return m_Entities.size();
}

Entity& Scene::GetEntity(
    std::size_t index)
{
    return *m_Entities[index];
}

const Entity& Scene::GetEntity(
    std::size_t index) const
{
    return *m_Entities[index];
}

} // namespace AtlasEngine