//
// Created by gaetz on 24/09/2024.
//

#ifndef GMATH_MAT4_HPP
#define GMATH_MAT4_HPP

#include "Vec3.hpp"

namespace gmath {

    class Mat4 {
    public:
        Mat4();
        explicit Mat4(const Matrix& raylibMatrix);
        Mat4 (f32 m0_, f32 m4_, f32 m8_, f32 m12_,
              f32 m1_, f32 m5_, f32 m9_, f32 m13_,
              f32 m2_, f32 m6_, f32 m10_, f32 m14_,
              f32 m3_, f32 m7_, f32 m11_, f32 m15_):
            m0 { m0_ }, m4 { m4_ }, m8 { m8_ }, m12 { m12_ },
            m1 { m1_ }, m5 { m5_ }, m9 { m9_ }, m13 { m13_ },
            m2 { m2_ }, m6 { m6_ }, m10 { m10_ }, m14 { m14_ },
            m3 { m3_ }, m7 { m7_ }, m11 { m11_ }, m15 { m15_ } {}

        f32 m0, m4, m8, m12;  // Matrix first row (4 components)
        f32 m1, m5, m9, m13;  // Matrix second row (4 components)
        f32 m2, m6, m10, m14; // Matrix third row (4 components)
        f32 m3, m7, m11, m15; // Matrix fourth row (4 components)

        Matrix ToRaylib() const;

        static Mat4 CreateRotationMatrix(const Vec3 &axis, f32 angle);
        static const Mat4 Identity;

    };
}

#endif //GMATH_MAT4_HPP
