//
// Created by gaetz on 06/09/2024.
//

#include "Vec3.hpp"
#include "Mat4.hpp"

namespace gmath
{
    Vec3::Vec3() :
        x { 0 },
        y { 0 },
        z { 0 } {}

    Vec3::Vec3(Vector3 raylibVec3) :
        x { raylibVec3.x },
        y { raylibVec3.y },
        z { raylibVec3.z } {}

    Vec3::Vec3(f32 x_, f32 y_, f32 z_) :
        x { x_ },
        y { y_ },
        z { z_ } {}

    const Vec3 Vec3::zero { 0.0f, 0.0f, 0.0f };
    const Vec3 Vec3::one { 1.0f, 1.0f, 1.0f };
    const Vec3 Vec3::up { 0.0f, 1.0f, 0.0f };
    const Vec3 Vec3::right { 1.0f, 0.0f, 0.0f };
    const Vec3 Vec3::forward { 0.0f, 0.0f, -1.0f };
    const Vec3 Vec3::down { 0.0f, -1.0f, 0.0f };
    const Vec3 Vec3::left { -1.0f, 0.0f, 0.0f };
    const Vec3 Vec3::backward { 0.0f, 0.0f, 1.0f };

    Vec3& Vec3::operator+=(const Vec3& r) {
        x += r.x;
        y += r.y;
        z += r.z;
        return *this;
    }

    Vec3& Vec3::operator-=(const Vec3& r) {
        x -= r.x;
        y -= r.y;
        z -= r.z;
        return *this;
    }

    Vec3& Vec3::operator*=(f32 scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vec3& Vec3::operator/=(const f32 scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    str Vec3::Log() const {
        return "{ x: " + std::to_string(x) + ", y:" + std::to_string(y) +
            ", z:" + std::to_string(z) + " }";
    }

    Vec3 Vec3::FromRaylib(Vector3 v) {
        return Vec3(v.x, v.y, v.z);
    }

    Vec3& Vec3::Normalize() {
        f32 length = Length();
        x /= length;
        y /= length;
        z /= length;
        return *this;
    }

    f32 Vec3::Length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    f32 Vec3::LengthSq() const {
        return x * x + y * y + z * z;
    }

    Vec3& Vec3::operator*=(const Mat4& mat) {
        x = mat.m0 * x + mat.m4 * y + mat.m8 * z + mat.m12;
        y = mat.m1 * x + mat.m5 * y + mat.m9 * z + mat.m13;
        z = mat.m2 * x + mat.m6 * y + mat.m10 * z + mat.m14;

        return *this;
    }

    Vec3 operator*(const Mat4& mat, const Vec3& r) {
        return Vec3 {
            mat.m0 * r.x + mat.m4 * r.y + mat.m8 * r.z + mat.m12,
            mat.m1 * r.x + mat.m5 * r.y + mat.m9 * r.z + mat.m13,
            mat.m2 * r.x + mat.m6 * r.y + mat.m10 * r.z + mat.m14
        };
    }

    // Rotates a vector around an axis
    Vec3& Vec3::RotateByAxisAngle(Vec3 axis, f32 angle) {
        // Using Euler-Rodrigues Formula
        // Ref.: https://en.wikipedia.org/w/index.php?title=Euler%E2%80%93Rodrigues_formula

        // Vector3Normalize(axis);
        float length = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
        if (length == 0.0f)
            length = 1.0f;
        float iLength = 1.0f / length;
        axis.x *= iLength;
        axis.y *= iLength;
        axis.z *= iLength;

        angle /= 2.0f;
        float a = sinf(angle);
        float b = axis.x * a;
        float c = axis.y * a;
        float d = axis.z * a;
        a = cosf(angle);
        Vector3 w = { b, c, d };

        // Vector3CrossProduct(w, v)
        Vector3 wv = { w.y * z - w.z * y, w.z * x - w.x * z, w.x * y - w.y * x };

        // Vector3CrossProduct(w, wv)
        Vector3 wwv = { w.y * wv.z - w.z * wv.y, w.z * wv.x - w.x * wv.z, w.x * wv.y - w.y * wv.x };

        // Vector3Scale(wv, 2*a)
        a *= 2;
        wv.x *= a;
        wv.y *= a;
        wv.z *= a;

        // Vector3Scale(wwv, 2)
        wwv.x *= 2;
        wwv.y *= 2;
        wwv.z *= 2;

        x += wv.x;
        y += wv.y;
        z += wv.z;

        x += wwv.x;
        y += wwv.y;
        z += wwv.z;

        return *this;
    }

    Vec3 Vec3::Cross(const Vec3& v1, const Vec3& v2) {
        return Vec3 { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
    }

    float Vec3::Angle(const Vec3& v1, const Vec3& v2) {
        float result = 0.0f;

        Vector3 cross = { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
        float len = sqrtf(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);
        float dot = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
        result = atan2f(len, dot);

        return result;
    }

    float Vec3::Distance(const Vec3& v1, const Vec3& v2) {
        return sqrtf((v1.x - v2.x) * (v1.x - v2.x) +
            (v1.y - v2.y) * (v1.y - v2.y) +
            (v1.z - v2.z) * (v1.z - v2.z));
    }

    float Vec3::DistanceSq(const Vec3& v1, const Vec3& v2) {
        return (v1.x - v2.x) * (v1.x - v2.x) +
            (v1.y - v2.y) * (v1.y - v2.y) +
            (v1.z - v2.z) * (v1.z - v2.z);
    }
}
