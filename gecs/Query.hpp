//
// Created by Gaëtan Blaise-Cazalet on 26/12/2023.
//

#ifndef GECS_QUERY_HPP
#define GECS_QUERY_HPP

#include <tuple>
#include <unordered_map>
#include "Defines.hpp"
#include "Types.hpp"
#include "World.hpp"
#include <functional>

namespace gecs {

    template<typename... ComponentTypes>
    class QueryCache {
    public:
        QueryCache() = default;
        explicit QueryCache(const std::tuple<vector<ComponentTypes>...>& newCache): cacheSoA(newCache) {
            size = std::get<0>(cacheSoA).size();
            cacheAoS.reserve(size);
            UpdateAoS();
            isPopulated = false;
        }

        template<typename FuncT>
        void ApplyOnElements(FuncT f) {
            for(std::size_t i = 0; i < size; ++i) {
                std::apply(f, cacheAoS[i]);
            }
            shouldRefresh = true;
        }

        template<typename FuncT>
        vector<Id> BuildDeleteList(FuncT f, const vector<Id>& entities) {
            vector<Id> toDelete;
            for(std::size_t i = 0; i < size; ++i) {
                if(std::apply(f, cacheAoS[i])) {
                    toDelete.push_back(entities[i]);
                }
            }
            return toDelete;
        }

        void ReintegrateInWorld(World& world) {
            if (shouldRefresh) {
                UpdateSoA();
                shouldRefresh = false;
            }
            world.ReintegrateQueryCache<ComponentTypes...>(cacheSoA);
        }

        bool IsPopulated() const { return isPopulated; }
        bool IsEmpty() const { return cacheAoS.size() == 0; }

        std::tuple<vector<ComponentTypes>...> cacheSoA;
        vector<std::tuple<ComponentTypes...>> cacheAoS;
        
    private:
        bool isPopulated{true};
        bool shouldRefresh{false};
        size_t size{0};

        void UpdateAoS() {
            cacheAoS.clear();
            for (size_t i = 0; i < size; ++i) {
                std::tuple<ComponentTypes...> tuple;
                std::apply([&](auto&&... comp) { ((comp = std::get<vector<std::decay_t<decltype(comp)>>>(cacheSoA)[i]), ...); }, tuple);
                cacheAoS.emplace_back(std::move(tuple));
            }
        }

        void UpdateSoA() {
            // Empty vectors but memory allocation is kept
            std::apply([](auto&&... vec) { (..., vec.clear()); }, cacheSoA);

            for (size_t i = 0; i < size; ++i) {
                std::apply([&](auto&&... vec) { (..., vec.push_back(std::get<ComponentTypes>(cacheAoS[i]))); }, cacheSoA);
            }
        }
    };


    template<typename... ComponentTypes>
    class Query {
        friend QueryCache<ComponentTypes...>;

    public:
        void Each(std::function<void(ComponentTypes&...)> f) {
            CheckCache();
            cache.ApplyOnElements(f);
        }

        void DeleteIf(std::function<bool(ComponentTypes&...)> f) {
            CheckCache();
            vector<Id> toDelete = cache.BuildDeleteList(f, entities);
            for(Id id : toDelete) {
                world.DestroyEntity(id);
                entities.erase(std::find(entities.begin(), entities.end(), id));
            }
        }

        void Apply() {
            if (cache.IsEmpty()) return;
            cache.ReintegrateInWorld(world);
        }


    private:
        World& world { World::Instance() };
        QueryCache<ComponentTypes...> cache;
        vector<Id> entities;

        void CheckCache() {
            if (cache.IsPopulated()) {
                auto result = world.Query<ComponentTypes...>();
                entities = get<0>(result);
                cache = QueryCache<ComponentTypes...>(tuple_tail(result));
            }
        }
    };
}
#endif //GECS_QUERY_HPP
