//
// Created by gaetz on 02/07/2023.
//

#include "Archetype.hpp"
#include "Components.hpp"
namespace ghecs {

    size_t Archetype::GetRowCount() const {
        if (components.empty()) return 0;
        return components[0].Count();
    }
}
