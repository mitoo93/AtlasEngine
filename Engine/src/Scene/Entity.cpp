#include "AtlasEngine/Scene/Entity.h"

namespace AtlasEngine
{

Entity::Entity(
    EntityID id,
    const std::string& name)
    : m_ID(id),
      m_Name(name)
{
    m_Transform.SetEntity(this);
}

const std::string& Entity::GetName() const
{
    return m_Name;
}

void Entity::SetName(
    const std::string& name)
{
    m_Name = name;
}

Transform& Entity::GetTransform()
{
    return m_Transform;
}

const Transform& Entity::GetTransform() const
{
    return m_Transform;
}

EntityID Entity::GetID() const
{
    return m_ID;
}

} // namespace AtlasEngine