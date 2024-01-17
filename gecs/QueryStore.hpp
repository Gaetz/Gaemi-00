//
// Created by gaetz on 17/01/24.
//

#ifndef GECS_QUERYSTORE_HPP
#define GECS_QUERYSTORE_HPP

#include "Types.hpp"

namespace gecs {

    class QueryStore {
        unordered_map<std::bitset<32>, AbstractQuery> queries;


    public:
        template<typename... ComponentTypes>
        void Store(std::tuple<vector<gecs::Id>, vector<ComponentTypes>...> res) {

        }
    };
}

#endif //GECS_QUERYSTORE_HPP
