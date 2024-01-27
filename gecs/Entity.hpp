//
// Created by gaetz on 02/07/2023.
//

#ifndef GECS_ENTITY_HPP
#define GECS_ENTITY_HPP

#include "Defines.hpp"
#include "Types.hpp"
#include "World.hpp"

namespace gecs {

    /**
     * Represents a game entity.
     */
    class Entity {

    public:
        Id id;
        explicit Entity(u64 idRoot);

        template<typename T>
        T& GetComponent() {
            return World::Instance().GetComponent<T>(id);
        }
    };



}


#endif //GECS_ENTITY_HPP
