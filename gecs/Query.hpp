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
#include "TupleUtils.hpp"

namespace gecs {

    template<typename... ComponentTypes>
    class QueryCache {
    public:
        QueryCache() = default;
        explicit QueryCache(const std::tuple<vector<ComponentTypes>...>& newCache): cacheSoA(newCache) {
            size = std::get<0>(cacheSoA).size();
            cacheAoS.reserve(size);
            UpdateAoS();
            isPopulated = true;
        }

        template<typename FuncT>
        void ApplyOnElements(FuncT f, bool updating) {
            for(std::size_t i = 0; i < size; ++i) {
                std::apply(f, cacheAoS[i]);
            }
            shouldRefresh = updating;
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

        [[nodiscard]] bool IsPopulated() const { return isPopulated; }
        [[nodiscard]] bool IsEmpty() const { return cacheAoS.empty(); }

        std::tuple<vector<ComponentTypes>...> cacheSoA;
        vector<std::tuple<ComponentTypes...>> cacheAoS;
        
    private:
        bool isPopulated{false};
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
        void Update(std::function<void(ComponentTypes&...)> f) {
            CheckCache();
            cache.ApplyOnElements(f, true);
        }

        void Read(std::function<void(const ComponentTypes&...)> f) {
            CheckCache();
            cache.ApplyOnElements(f, false);
        }

        void DeleteIf(std::function<bool(const ComponentTypes&...)> f) {
            if (cache.IsEmpty()) return;

            vector<Id> toDelete = cache.BuildDeleteList(f, entities);
            if (toDelete.empty()) return;

            for(Id id : toDelete) {
                world.DestroyEntity(id);
            }
            Refresh();
        }

        void Apply() {
            if (cache.IsEmpty()) return;
            cache.ReintegrateInWorld(world);
        }

        void Refresh() {
            if (cache.IsEmpty()) return;
            RefreshCache();
        }

    private:
        World& world { World::Instance() };
        static inline QueryCache<ComponentTypes...> cache {};
        static inline vector<Id> entities {};

        void RefreshCache() {
            auto result = world.Query<ComponentTypes...>();
            entities = get<0>(result);
            cache = QueryCache<ComponentTypes...>(TupleTail(result));
        }

        void CheckCache() {
            if (!cache.IsPopulated()) {
                RefreshCache();
            }
        }
    };
}
#endif //GECS_QUERY_HPP
