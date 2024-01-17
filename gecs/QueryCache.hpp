//
// Created by gaetz on 17/01/24.
//

#ifndef GECS_QUERYCACHE_HPP
#define GECS_QUERYCACHE_HPP

#include "Defines.hpp"
#include "Types.hpp"

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

        const std::tuple<vector<ComponentTypes>...>& RefreshAndReturnCache() {
            if (shouldRefresh) {
                UpdateSoA();
                shouldRefresh = false;
            }
            return cacheSoA;
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

}

#endif //GECS_QUERYCACHE_HPP
