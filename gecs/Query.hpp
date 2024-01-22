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
    class Query : public AbstractQuery {
    public:
        Query() = default;

        explicit Query(std::tuple<vector<gecs::Id>, vector<ComponentTypes>...> res) {
            entities = get<0>(res);
            cache = QueryCache<ComponentTypes...>(TupleTail(res));
        }

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

            QueryManager::Instance().DestroyEntities(toDelete);
            Refresh();
        }

        void Apply() {
            if (cache.IsEmpty()) return;
            const auto& lastCache = cache.RefreshAndReturnCache();
            QueryManager::Instance().ReintegrateQueryCache<ComponentTypes...>(lastCache);
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

        void CheckCache() {
            if (!cache.IsPopulated()) {
                RefreshCache();
            }
        }
    };
}
#endif //GECS_QUERY_HPP
