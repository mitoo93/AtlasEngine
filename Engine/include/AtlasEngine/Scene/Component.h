#pragma once

namespace AtlasEngine
{

class Entity;

class Component
{
public:

    virtual ~Component() = default;

    Entity* GetEntity() const;

private:

    friend class Entity;

    void SetEntity(Entity* entity);

private:

    Entity* m_Entity = nullptr;
};

} // namespace AtlasEngine