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
    };

    str LogComponent(ComponentId componentId, const Column& column, size_t row);

    class Entity;
}




/*
class PositionColumn : public Column {
public:
    gecs::ComponentId id { gecs::ComponentId::Position };
    vector<Position> data;

    void* GetDataRow(size_t row) override {
        return &data[row];
    };

    u64 AddElement(void* newData, size_t sizeOfData) override {
        GASSERT_MSG(sizeof(Position) == sizeOfData, "New element size must be size of component column type (Position)")
        auto* newDataPtr = static_cast<Position*>(newData);
        Position newElement;
        newElement.x = newDataPtr->x;
        newElement.y = newDataPtr->y;
        data.push_back(newElement);
        return data.size() - 1;
    }

    void RemoveElement(u64 row) override {
        data.erase(data.begin() + row);
    };
};
 */



#endif //GECS_COMPONENTS_HPP
