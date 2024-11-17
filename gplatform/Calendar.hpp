//
// Created by gaetz on 16/05/2023.
//

#ifndef GPLATFORM_CALENDAR_HPP
#define GPLATFORM_CALENDAR_HPP

#include "Defines.hpp"

namespace gplatform {
    /**
     * Get date on 19 chars, for logging purpose.
     * Platform abstraction.
     * @return Date on 19 chars
     */
    array<char, 19> GetDate();
}

#endif //GPLATFORM_CALENDAR_HPP
