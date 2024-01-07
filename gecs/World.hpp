//
// Created by gaetz on 03/07/2023.
//

#ifndef GECS_WORLD_HPP
#define GECS_WORLD_HPP

#include "Types.hpp"
#include "Archetype.hpp"
#include <tuple>
#include <ranges>
#include "QueryManager.hpp"

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
        unordered_map<ComponentId, ComponentArchetypes>& GetComponents() { return componentRegistry; }


        void Init();
        Id CreateEntity();
        void DestroyEntity(Id entityId);
        static Entity GetEntity(Id entityId);
        Archetype* GetArchetype(const str& archetypeName);
        void LogWorld();

        template<typename... ComponentTypes>
        std::tuple<vector<Id>, vector<ComponentTypes>...> Query() {
            return queryManager.Query<ComponentTypes...>();
        }

        template<typename... ComponentTypes>
        void ReintegrateQueryCache(std::tuple<vector<ComponentTypes>...> tuple) {
            vector<CompArchIdAndCol> compArchCols = queryManager.GetRelevantArchetypesAndCols<ComponentTypes...>();
            // From compArchCols, Get vector of vector of pairs of archetypes id and cols,one vector for each component type
            auto archsAndCols = GetArchetypeAndColumnIndices(compArchCols);
            // Get start indices for each archetype containing the component type
            vector<vector<size_t>> starts = GetDataStartIndices(compArchCols);
            // Recursively call ReintegrateDataInColumn for each component type
            size_t i { 0 };
            size_t j { 0 };
            std::apply([&](auto&&... data) { (ReintegrateDataInColumn(data, starts[i++], archsAndCols[j++]), ...); }, tuple);
        }

        template<typename T>
        void ReintegrateDataInColumn(const std::vector<T>& tupleData, vector<size_t> start, vector<std::pair<ArchetypeId, size_t>> archsAndCols) {
            for (u32 i = 0; i < start.size(); ++i) {
                const auto archId = archsAndCols[i].first;
                const auto column = archsAndCols[i].second;
                const auto count = archetypeRegistry[archId].components[column].Count();
                auto it = tupleData.begin() + start[i];
                auto end = tupleData.begin() + start[i] + count;
                vector<T> newData = std::vector<T>(it, end);
                archetypeRegistry[archId].components[column].ReplaceData<T>(newData);
            }
        }


    private:
        u64 maxId { 0 };
        ArchetypeId defaultArchetype;
        QueryManager queryManager;

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

        u64 MoveEntity(const ArchetypeRecord& recordToUpdate, size_t row, Archetype* nextArchetype);

        static vector<vector<std::pair<ArchetypeId, size_t>>> GetArchetypeAndColumnIndices(const vector<CompArchIdAndCol> &compArchCols);

        vector<vector<size_t>> GetDataStartIndices(vector<CompArchIdAndCol> &compArchCols);

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
