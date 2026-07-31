#include "AtlasEngine/Scene/Entity.h"

namespace AtlasEngine
{

Entity::Entity(
    EntityID id,
    const std::string& name)
    :
    m_ID(id),
    m_Name(name)
{
    AddComponent<Transform>();
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
    return GetComponent<Transform>();
}

const Transform& Entity::GetTransform() const
{
    return GetComponent<Transform>();
}

EntityID Entity::GetID() const
{
    return m_ID;
}

} // namespace AtlasEngine