#include "AtlasEngine/Scene/Component.h"

namespace AtlasEngine
{

Entity* Component::GetEntity() const
{
    return m_Entity;
}

void Component::SetEntity(Entity* entity)
{
    m_Entity = entity;
}

} // namespace AtlasEngine