//
// Created by gaetz on 03/07/2023.
//

#ifndef GECS_WORLD_HPP
#define GECS_WORLD_HPP

#include "Types.hpp"
#include "Archetype.hpp"
#include <tuple>
#include <ranges>
#include "Query.hpp"

namespace gecs {

    class World {
        friend class Entity;

    private:
        /**
         * Each entity has an a archetype reference
         */
        unordered_map<Id, ArchetypeRecord> entityRegistry;

        /**
         * Each archetype can be found through its list of components
         */
        unordered_map<ArchetypeId, Archetype> archetypeRegistry;

        /**
         * In which archetypes (and column inside archetype) we can find each component type
         */
        using ComponentArchetypes = unordered_map<ArchetypeId, size_t>;
        unordered_map<ComponentId, ComponentArchetypes> componentRegistry;

    public:
        unordered_map<Id, ArchetypeRecord>& GetEntities() { return entityRegistry; }
        unordered_map<ArchetypeId, Archetype>& GetArchetypes() { return archetypeRegistry; }
        [[nodiscard]] const unordered_map<ArchetypeId, Archetype>& GetArchetypesConst() const { return archetypeRegistry; }
        unordered_map<ComponentId, ComponentArchetypes>& GetComponents() { return componentRegistry; }


        void Init();
        Id CreateEntity();
        void DestroyEntity(Id entityId);
        static Entity GetEntity(Id entityId);
        Archetype* GetArchetype(const str& archetypeName);
        void LogWorld();

        template<typename ...ComponentTypes>
        Query<ComponentTypes...>& Find() {
            QueryManager& queryManager = QueryManager::Instance();
            ArchetypeId archId = ToArchetypeId<ComponentTypes...>();
            if (!queryManager.HasQuery(archId)) {
                void* query = new Query<ComponentTypes...>();
                queryManager.Store(archId, query);
            }
            auto ret = reinterpret_cast<Query<ComponentTypes...>*>(queryManager.Get(archId));
            return *ret;
        }

        template<class T>
        void AddComponent(Id id, T componentData) {
            ComponentId component = ToComponentId<T>();
            ArchetypeRecord& recordToUpdate = GetEntities()[id];
            Archetype* nextArchetype = recordToUpdate.archetype->archetypeChanges[component].add;
            // Add data in component column of new archetype
            u64 newRow;
            for (auto& column : nextArchetype->components) {
                if (component != column.GetComponentId()) continue;
                newRow = column.AddElement<T>(componentData);
                break;
            }
            // Move previous archetype data in new archetype
            if (recordToUpdate.archetype->archetypeId != 0) {
                MoveEntity(recordToUpdate, recordToUpdate.row, nextArchetype);
            }

            // Update entity's row
            recordToUpdate.archetype = nextArchetype;
            recordToUpdate.row = newRow;
        }

        void RemoveComponent(Id entityId, ComponentId componentId);

        template<class T>
        T& GetComponent(Id entity) {
            const ComponentId componentId = ToComponentId<T>();
            const ArchetypeRecord& record = entityRegistry[entity];
            Archetype* archetype = record.archetype;

            // Assert archetype has component
            ComponentArchetypes archetypes = componentRegistry[componentId];
            GASSERT_MSG(archetypes.count(archetype->archetypeId) != 0, "Archetype must have component to get it")

            const size_t componentColumn = archetypes[archetype->archetypeId];
            return archetype->components[componentColumn].GetRow<T>(record.row);
        }

    private:
        u64 maxId { 0 };
        ArchetypeId defaultArchetype;

        u64 MoveEntity(const ArchetypeRecord& recordToUpdate, size_t row, Archetype* nextArchetype);

        // Singleton
    private:
        World() = default;

    public:
        static World& Instance() {
            static World instance;
            return instance;
        }

        World(World const&) = delete;
        void operator=(World const&) = delete;
    };

}


#endif //GECS_WORLD_HPP
