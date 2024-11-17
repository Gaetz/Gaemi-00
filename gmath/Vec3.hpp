//
// Created by gaetz on 06/09/2024.
//

#ifndef GMATH_VEC3_HPP
#define GMATH_VEC3_HPP

#include "Types.hpp"
#include "raylib.h"
#include "GMath.hpp"

namespace gmath {
    class Mat4;

    class Vec3 {
    public:
        f32 x;
        f32 y;
        f32 z;

        Vec3();

        explicit Vec3(Vector3 raylibVec3);
        explicit Vec3(f32 x_, f32 y_, f32 z_);

        [[nodiscard]] str Log() const;

        static const Vec3 zero;
        static const Vec3 one;
        static const Vec3 up;
        static const Vec3 right;
        static const Vec3 forward;
        static const Vec3 down;
        static const Vec3 left;
        static const Vec3 backward;

        // Arithmetic operators

        Vec3 &operator+=(const Vec3 &r);

        Vec3 &operator-=(const Vec3 &r);

        Vec3 &operator*=(f32 scalar);

        Vec3 &operator/=(f32 scalar);

        Vec3 &operator*=(const Mat4& scalar);

        friend Vec3 operator+(const Vec3 &l, const Vec3 &r) {
            return Vec3 { l.x + r.x, l.y + r.y, l.z + r.z };
        }

        friend Vec3 operator-(const Vec3 &l, const Vec3 &r) {
            return Vec3 { l.x - r.x, l.y - r.y, l.z - r.z };
        }

        friend Vec3 operator*(const Vec3 &l, const f32 scalar) {
            return Vec3 { l.x * scalar, l.y * scalar, l.z * scalar };
        }

        friend Vec3 operator*(const f32 scalar, const Vec3 &r) {
            return Vec3 { r.x * scalar, r.y * scalar, r.z * scalar };
        }

        friend Vec3 operator/(const Vec3 &l, const f32 scalar) {
            return Vec3 { l.x / scalar, l.y / scalar, l.z / scalar };
        }

        friend Vec3 operator/(const f32 scalar, const Vec3 &r) {
            return Vec3 { r.x / scalar, r.y / scalar, r.z / scalar };
        }

        friend Vec3 operator*(const Mat4& mat, const Vec3 &r);

        static Vec3 Cross(const Vec3& v1, const Vec3& v2);
        static float Angle(const Vec3& v1, const Vec3& v2);
        static float Distance(const Vec3& v1, const Vec3& v2);
        static float DistanceSq(const Vec3& v1, const Vec3& v2);

        f32 Length() const;
        f32 LengthSq() const;
        Vec3& RotateByAxisAngle(Vec3 axis, f32 angle);
        Vec3& Normalize();

        // Conversions
        Vector3 ToRaylib() const {
            return Vector3 { x, y, z };
        }

        static Vec3 FromRaylib(Vector3 vector3);
    };
}

#endif //GMATH_VEC3_HPP
