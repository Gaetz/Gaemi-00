//
// Created by gaetz on 24/09/2024.
//

#ifndef GMATH_MAT4_HPP
#define GMATH_MAT4_HPP

#include "Vec3.hpp"

namespace gmath {

    class Mat4 {
    public:
        float m0, m4, m8, m12;  // Matrix first row (4 components)
        float m1, m5, m9, m13;  // Matrix second row (4 components)
        float m2, m6, m10, m14; // Matrix third row (4 components)
        float m3, m7, m11, m15; // Matrix fourth row (4 components)

        Matrix ToRaylib() const;

        static Mat4 CreateRotationMatrix(const Vec3 &axis, float angle);
        static const Mat4 Identity;

    };
}

#endif //GMATH_MAT4_HPP
