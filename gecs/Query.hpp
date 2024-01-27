//
// Created by Gaëtan Blaise-Cazalet on 26/12/2023.
//

#ifndef GECS_QUERY_HPP
#define GECS_QUERY_HPP

#include <tuple>
#include <unordered_map>
#include "Defines.hpp"
#include "Types.hpp"
#include <functional>
#include "TupleUtils.hpp"
#include "QueryCache.hpp"
#include "QueryManager.hpp"

namespace gecs {

    template<typename... ComponentTypes>
    class Query {
    public:
        Query() {
            RefreshCache();
        }

        void Update(std::function<void(ComponentTypes&...)> f) {
            cache.ApplyOnElements(f, true);
        }

        void Read(std::function<void(const ComponentTypes&...)> f) {
            cache.ApplyOnElements(f, false);
        }

        void DeleteIf(std::function<bool(const ComponentTypes&...)> f) {
            if (cache.IsEmpty()) return;

            vector<Id> toDelete = cache.BuildFilteredEntityList(f, entities);
            if (toDelete.empty()) return;

            QueryManager::Instance().DestroyEntities(toDelete);
            Refresh();
        }

        template<class Component>
        void RemoveIf(std::function<bool(const ComponentTypes&...)> f) {
            if (cache.IsEmpty()) return;

            vector<Id> removeCompEntities = cache.BuildFilteredEntityList(f, entities);
            if (removeCompEntities.empty()) return;

            // Apply so last modification (in AoS) is reintegrated in SoA and world
            Apply();

            auto& manager = QueryManager::Instance();
            for (const auto& toRemove : removeCompEntities) {
                manager.RemoveComponentFromEntity<Component>(toRemove);
            }

            Refresh();
        }

        void Apply() {
            if (cache.IsEmpty()) return;
            const auto& lastCache = cache.RefreshAndReturnCache();
            QueryManager::Instance().ReintegrateQueryCache<ComponentTypes...>(lastCache);
            /// TODO Could also update the cache of other queries
        }

        void Refresh() {
            if (cache.IsEmpty()) return;
            RefreshCache();
        }


    private:
        QueryCache<ComponentTypes...> cache {};
        vector<Id> entities {};

        void RefreshCache() {
            auto result = QueryManager::Instance().ComputeQuery<ComponentTypes...>();
            entities = get<0>(result);
            cache = QueryCache<ComponentTypes...>(TupleTail(result));
        }
    };
}
#endif //GECS_QUERY_HPP
