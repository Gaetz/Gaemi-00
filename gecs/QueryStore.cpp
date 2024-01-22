//
// Created by gaetz on 17/01/24.
//

#include "QueryStore.hpp"

namespace gecs {

    void QueryStore::Store(gecs::ArchetypeId key, gecs::AbstractQuery &&q) {
        queries.emplace(key, std::move(q));
    }

    bool QueryStore::HasQuery(ArchetypeId archId) {
        return queries.contains(archId);
    }
}