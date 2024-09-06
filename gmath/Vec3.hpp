//
// Created by gaetz on 06/09/2024.
//

#ifndef GMATH_VEC3_HPP
#define GMATH_VEC3_HPP

#include "Types.hpp"
#include "raylib.h"
#include "GMath.hpp"

namespace gmath {

    class Vec3 {
    public:
        f32 x;
        f32 y;
        f32 z;

        Vec3();
        explicit Vec3(f32 x, f32 y, f32 z);
        [[nodiscard]] str Log() const;

        // Arithmetic operators

        Vec3& operator+=(const Vec3& r);
        Vec3& operator-=(const Vec3& r);
        Vec3& operator*=(f32 scalar);
        Vec3& operator/=(f32 scalar);

        friend Vec3 operator+(const Vec3& l, const Vec3& r) {
            return Vec3 { l.x + r.x, l.y + r.y, l.z + r.z };
        }

        friend Vec3 operator-(const Vec3& l, const Vec3& r) {
            return Vec3 { l.x - r.x, l.y - r.y, l.z - r.z };
        }

        friend Vec3 operator*(const Vec3& l, const f32 scalar) {
            return Vec3 { l.x * scalar, l.y * scalar, l.z * scalar };
        }

        friend Vec3 operator*(const f32 scalar, const Vec3& r) {
            return Vec3 { r.x * scalar, r.y * scalar, r.z * scalar };
        }

        friend Vec3 operator/(const Vec3& l, const f32 scalar) {
            return Vec3 { l.x / scalar, l.y / scalar, l.z / scalar };
        }

        friend Vec3 operator/( const f32 scalar, const Vec3& r) {
            return Vec3 { r.x / scalar, r.y / scalar, r.z / scalar };
        }

        // Conversions
        Vector3 ToRaylib() {
            return Vector3 { x, y, z };
        }
    };
}

#endif //GMATH_VEC3_HPP
