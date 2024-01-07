//
// Created by Gaëtan Blaise-Cazalet on 18/12/2023.
//

#include "Defines.hpp"

#ifndef GPLATFORM_STRINGUTILS_HPP
#define GPLATFORM_STRINGUTILS_HPP

namespace gplatform {
    void StrCpy(char* dst, u32 length, const char* src);
    size_t StrnLen(char* str, size_t max);
    size_t StrLen(char* str);
}

#endif //GPLATFORM_STRINGUTILS_HPP