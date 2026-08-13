#include "AtlasEngine/Scene/Component.h"

namespace AtlasEngine
{

void Component::SetEntity(Entity* entity)
{
    m_Entity = entity;
}

} // namespace AtlasEngine