//
// Created by gaetz on 02/07/2023.
//

#ifndef GECS_ARCHETYPE_HPP
#define GECS_ARCHETYPE_HPP

#include "Types.hpp"
#include "Column.hpp"

namespace gecs {

    class Archetype;

    struct ArchetypeChange {
        Archetype* add;
        Archetype* remove;
    };

    class Archetype {
    public:
        Id id;
        ArchetypeId archetypeId;
        char name[12];
        vector<Column> components;
        unordered_map<ComponentId, ArchetypeChange> archetypeChanges;

        size_t GetRowCount() const;

    };



}


#endif //GECS_ARCHETYPE_HPP
