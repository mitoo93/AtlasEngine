#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <stdexcept>

#include "AtlasEngine/Scene/Entity.h"
#include "AtlasEngine/Systems/System.h"
#include "AtlasEngine/Scene/CameraComponent.h"

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

    const std::vector<std::unique_ptr<Entity>>& GetEntities() const;

    Entity* FindEntityByName(
        const std::string& name);

    const Entity* FindEntityByName(
        const std::string& name) const;

    template<typename T, typename... Args>
    T& AddSystem(Args&&... args)
    {
        static_assert(
            std::is_base_of_v<System, T>,
            "T must derive from System");

        auto system =
            std::make_unique<T>(
                std::forward<Args>(args)...);

        T* ptr = system.get();

        m_Systems.push_back(
            std::move(system));

        return *ptr;
    }

    template<typename T>
    T& GetSystem()
    {
        for (auto& system : m_Systems)
        {
            if (auto ptr =
                dynamic_cast<T*>(system.get()))
            {
                return *ptr;
            }
        }

        throw std::runtime_error(
            "System not found");
    }

    void Update(
        Shader& shader);

    void SetActiveCamera(
        CameraComponent* camera);

    CameraComponent* GetActiveCamera();

    const CameraComponent* GetActiveCamera() const;

private:

    EntityID m_NextEntityID = 1;
    std::vector<std::unique_ptr<Entity>> m_Entities;
    std::vector<std::unique_ptr<System>> m_Systems;
    CameraComponent* m_ActiveCamera = nullptr;
    
};

} // namespace AtlasEngine