#ifndef GAMEIMPL_ENTITY_HPP
#define GAMEIMPL_ENTITY_HPP

#include <array>
#include "Defines.hpp"

namespace gecs {

    constexpr i32 MAX_COMPONENTS = 3;

    struct Entity {
        explicit Entity(u64 idP) : id{idP} {}

        u64 id;
        std::array<i32, MAX_COMPONENTS> components{-1, -1, -1};
    };

}
#endif //GAMEIMPL_ENTITY_HPP