//
// Created by gaetz on 02/07/2023.
//

#ifndef GECSS_TYPES_HPP
#define GECSS_TYPES_HPP

#include <bitset>
using std::bitset;

#include <unordered_map>
using std::unordered_map;

#include "Defines.hpp"
#include "Defines.hpp"

namespace gecs {

    constexpr u32 MAX_COMPONENTS = 32;
    constexpr u64 LAST_32_BITS = 0b0000000000000000000000000000000011111111111111111111111111111111;

    using Id = u64;
    using ArchetypeId = bitset<MAX_COMPONENTS>;

    struct Position;
    struct Velocity;
    struct Sprite;

    // This list of components is used to create the column
    // type, with a std::variant.
    #define COMPONENTS Position, Velocity, Sprite


    enum class ComponentId {
        Position = 0,
        Velocity = 1,
        Sprite = 2,
    };

    str ComponentIdToString(ComponentId componentId);
    ComponentId IndexToComponentId(i32 index);
    str ArchetypeIdToString(ArchetypeId id);
    u32 ComponentIdToBitValue(ComponentId componentId);


    class Archetype;

    struct ArchetypeRecord {
        /// Reference to entity's archetype
        Archetype* archetype;
        /// Entity row in archetypes component table
        u64 row;
    };

    struct CompArchIdAndCol {
        ComponentId componentId { ComponentId::Position };
        ArchetypeId archId { 0 };
        size_t columnIndex { 0 };
    };

    struct IdArchRow {
        Id id {0};
        ArchetypeId archId;
        size_t row {0};
    };


    template <class T>
    ComponentId ToComponentId() {
        if constexpr (std::is_same_v<T, Position>) {
            return ComponentId::Position;
        } else if constexpr (std::is_same_v<T, Velocity>) {
            return ComponentId::Velocity;
        } else if constexpr (std::is_same_v<T, Sprite>) {
            return ComponentId::Sprite;
        }
    }

    template <typename... ComponentTypes>
    std::vector<ComponentId> ToComponentIds() {
        std::vector<ComponentId> ret;

        // Use fold expression to call ToComponentId for each type
        (ret.push_back(ToComponentId<ComponentTypes>()), ...);

        return ret;
    }

    template <typename... ComponentTypes>
    ArchetypeId ToArchetypeId() {
        std::vector<ComponentId> comps = ToComponentIds<ComponentTypes...>();
        ArchetypeId id;
        for (auto comp : comps) {
            id.set(static_cast<size_t>(comp));
        }
        return id;
    }

    class AbstractQuery {
    public:
        virtual ~AbstractQuery() = default;
    };
}

#endif //GECSS_TYPES_HPP
