//
// Created by gaetz on 16/05/2023.
//

#ifndef GPLATFORM_CONSOLELINE_HPP
#define GPLATFORM_CONSOLELINE_HPP

#include "Defines.hpp"

namespace gplatform {
    /**
     * Write a message to the console.
     * Platform abstraction.
     * @param message The message you want to write
     * @param level Severity level
     */
    void ConsoleWrite(const str& message, i32 level);

}

#endif //GPLATFORM_CONSOLELINE_HPP
