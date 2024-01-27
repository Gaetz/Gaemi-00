//
// Created by gaetz on 02/07/2023.
//

#ifndef GECS_COMPONENTS_HPP
#define GECS_COMPONENTS_HPP

#include <variant>
#include "Types.hpp"
#include "Asserts.hpp"
#include "Log.hpp"
#include <algorithm>

#include "Rect.hpp"
using gmath::Rect;
using gmath::Vec2;

#include "unordered_map"
using std::unordered_map;

namespace gecs {

    struct Column;

    struct Position {
        f32 x { 0.0f };
        f32 y { 0.0f };

        void Set(const Vec2& newVec) {
            x = newVec.x;
            y = newVec.y;
        }

        [[nodiscard]] Vec2 Vec() const {
            return Vec2 { x, y };
        }
    };

    struct Velocity {
        f32 x { 0.0f };
        f32 y { 0.0f };

        [[nodiscard]] Vec2 Vec() const {
            return Vec2 { x, y };
        }
    };

    struct Sprite {

        Sprite() = default;
        explicit Sprite(Texture textureP):
            srcRect { 0.0f, 0.0f, static_cast<f32>(textureP.width), static_cast<f32>(textureP.height) },
            dstSize { srcRect.width, srcRect.height },
            texture { textureP }
        {}

        Rect srcRect;
        Vec2 dstSize { 1.0f, 1.0f };
        Texture texture {};
        bool visible { true };
    };

    str LogComponent(ComponentId componentId, const Column& column, size_t row);

    class Entity;
}


#endif //GECS_COMPONENTS_HPP
