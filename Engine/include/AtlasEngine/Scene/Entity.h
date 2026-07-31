#pragma once

#include <string>

#include "AtlasEngine/Math/Transform.h"
#include "AtlasEngine/Scene/EntityID.h"

namespace AtlasEngine
{

class Entity
{
public:

    Entity() = default;

    Entity(
        EntityID id,
        const std::string& name);

    EntityID GetID() const;

    const std::string& GetName() const;
    void SetName(const std::string& name);

    Transform& GetTransform();
    const Transform& GetTransform() const;

private:

    EntityID m_ID = InvalidEntity;

    std::string m_Name;

    Transform m_Transform;
};

} // namespace AtlasEngine