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

const std::vector<std::unique_ptr<Entity>>& Scene::GetEntities() const
{
    return m_Entities;
}

Entity* Scene::FindEntityByName(
    const std::string& name)
{
    for (auto& entity : m_Entities)
    {
        if (entity->GetName() == name)
        {
            return entity.get();
        }
    }

    return nullptr;
}

const Entity* Scene::FindEntityByName(
    const std::string& name) const
{
    for (const auto& entity : m_Entities)
    {
        if (entity->GetName() == name)
        {
            return entity.get();
        }
    }

    return nullptr;
}

} // namespace AtlasEngine