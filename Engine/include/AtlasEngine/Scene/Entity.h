#pragma once

#include <stdexcept>
#include <memory>
#include <type_traits>
#include <vector>

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

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        static_assert(
            std::is_base_of_v<Component, T>,
            "T must derive from Component");

        auto component =
            std::make_unique<T>(
                std::forward<Args>(args)...);;

        T* ptr = component.get();

        m_Components.push_back(
            std::move(component));

        return *ptr;
    }

    template<typename T>
    T& GetComponent()
    {
        static_assert(
            std::is_base_of_v<Component, T>,
            "T must derive from Component");

        for (auto& component : m_Components)
        {
            if (auto ptr =
                dynamic_cast<T*>(component.get()))
            {
                return *ptr;
            }
        }

        throw std::runtime_error(
            "Component not found");
    }

    template<typename T>
    const T& GetComponent() const
    {
        static_assert(
            std::is_base_of_v<Component, T>,
            "T must derive from Component");

        for (const auto& component : m_Components)
        {
            if (auto ptr =
                dynamic_cast<const T*>(component.get()))
            {
                return *ptr;
            }
        }

        throw std::runtime_error(
            "Component not found");
    }

    template<typename T>
    bool HasComponent() const
    {
        static_assert(
            std::is_base_of_v<Component, T>,
            "T must derive from Component");

        for (const auto& component : m_Components)
        {
            if (dynamic_cast<const T*>(component.get()))
            {
                return true;
            }
        }

        return false;
    }

private:

    EntityID m_ID = InvalidEntity;

    std::string m_Name;

    std::vector<std::unique_ptr<Component>> m_Components;
};

} // namespace AtlasEngine