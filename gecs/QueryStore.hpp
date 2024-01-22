//
// Created by gaetz on 17/01/24.
//

#ifndef GECS_QUERYSTORE_HPP
#define GECS_QUERYSTORE_HPP

#include "Types.hpp"
#include "Query.hpp"

namespace gecs {

    class QueryStore {
        unordered_map<ArchetypeId, AbstractQuery> queries;
    public:
        void Store(ArchetypeId key, AbstractQuery &&q);

        /*
        template<class ...ComponentTypes>
        Query<ComponentTypes...> &Get() {
            auto archId = ToArchetypeId<ComponentTypes...>();
            return dynamic_cast<Query<ComponentTypes...>>(queries[archId]);
        }
        */

        AbstractQuery& Get(ArchetypeId key) {
            return queries[key];
        }


        bool HasQuery(ArchetypeId archId);
    };
}

#endif //GECS_QUERYSTORE_HPP
