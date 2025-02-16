//
// Created by gaetz on 02/07/2023.
//

#ifndef GHECS_ARCHETYPE_HPP
#define GHECS_ARCHETYPE_HPP

#include "Types.hpp"
#include "Column.hpp"

namespace ghecs {

    class Archetype;

    /**
     * Each archetype hold the next graph position when
     * trying to add or remove a specific component to it.
     */
    struct ArchetypeChange {
        Archetype* add;
        Archetype* remove;
    };

    /**
     * An Archetype stores a specific set of component vectors,
     * corresponding to an entity of specific composition. Each
     * entity data is stored on the same "row", meaning at the same
     * index for each component vector.
     *
     * For practical purpose, it has an Id and a char array name.
     * Maybe the Id should be discarded.
     */
    class Archetype {
    public:
        Id id;
        ArchetypeId archetypeId;
        char name[12];
        vector<Column> components;

        /**
         * For a specific component added or removed (key), where to
         * go in the archetype graph (value).
         */
        unordered_map<ComponentId, ArchetypeChange> archetypeChanges;

        /**
         * Return number of rows for components' vector.
         * Each vector has the same number of rows.
         * @return Number of rows
         */
        size_t GetRowCount() const;

    };



}


#endif //GHECS_ARCHETYPE_HPP
