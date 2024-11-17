//
// Created by gaetz on 07/02/2024.
//

#ifndef GMATH_VEC4_HPP
#define GMATH_VEC4_HPP

#include "Types.hpp"
#include "raylib.h"
#include "GMath.hpp"

namespace gmath {

    class Vec4 {
    public:
        f32 x;
        f32 y;
        f32 z;
        f32 w;

        Vec4();
        explicit Vec4(f32 x, f32 y, f32 z, f32 w);
        [[nodiscard]] str Log() const;

        // Arithmetic operators

        Vec4& operator+=(const Vec4& r);
        Vec4& operator-=(const Vec4& r);
        Vec4& operator*=(f32 scalar);
        Vec4& operator/=(f32 scalar);

        friend Vec4 operator+(const Vec4& l, const Vec4& r) {
            return Vec4 { l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w };
        }

        friend Vec4 operator-(const Vec4& l, const Vec4& r) {
            return Vec4 { l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w };
        }

        friend Vec4 operator*(const Vec4& l, const f32 scalar) {
            return Vec4 { l.x * scalar, l.y * scalar, l.z * scalar, l.w * scalar };
        }

        friend Vec4 operator*(const f32 scalar, const Vec4& r) {
            return Vec4 { r.x * scalar, r.y * scalar, r.z * scalar, r.w * scalar };
        }

        friend Vec4 operator/(const Vec4& l, const f32 scalar) {
            return Vec4 { l.x / scalar, l.y / scalar, l.z / scalar, l.w / scalar };
        }

        friend Vec4 operator/( const f32 scalar, const Vec4& r) {
            return Vec4 { r.x / scalar, r.y / scalar, r.z / scalar, r.w / scalar };
        }

        // Conversions
        Vector4 ToRaylib() {
            return Vector4 { x, y, z, w };
        }
    };
}

#endif //GMATH_VEC4_HPP
