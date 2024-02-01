//
// Created by Gaëtan Blaise-Cazalet on 22/01/2024.
//

#ifndef GECS_COLUMN_HPP
#define GECS_COLUMN_HPP

#include "Types.hpp"
#include "Components.hpp"
namespace gecs {

    struct Position;
    struct Velocity;
    struct Sprite;

    class Column {
    private:
        u32 dataSize;
        ComponentId componentId;
        vector<std::variant<COMPONENTS>> data;

    public:
        template<class T>
        void Init(size_t numberOfElements = 100) {
            data.reserve(numberOfElements);
            dataSize = sizeof(T);
            componentId = ToComponentId<T>();
        }

        template<class T>
        T& GetRow(size_t row) {
            return std::get<T>(data[row]);
        }

        template<class T>
        const T& GetRowConst(size_t row) const {
            return std::get<T>(data[row]);
        }

        template<class T>
        void ReplaceData(const vector<T>&& newData) {
            GASSERT_DEBUG(data.size() == newData.size())
            data.assign(newData.begin(), newData.end());
        }

        [[nodiscard]] size_t Count() const  { return data.size(); }
        [[nodiscard]] ComponentId GetComponentId() const { return componentId; }


        template<class T>
        u64 AddElement(T element) {
            data.push_back(std::move(element));
            return data.size() - 1;
        };

        void RemoveElementBySwapping(u64 row) {
            auto last = data.end() - 1;
            auto removedElement = data.begin() + static_cast<i64>(row);
            std::iter_swap(removedElement, last);
            data.pop_back();
        };

        /* Functions used for logging purpose */

        [[nodiscard]] const Position& GetPos(size_t row) const;
        [[nodiscard]] const Velocity& GetVelocity(size_t row) const;
        [[nodiscard]] const Sprite& GetSprite(size_t row) const;
    };

} // gecs

#endif //GAEMI_00_COLUMN_HPP
