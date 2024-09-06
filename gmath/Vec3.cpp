//
// Created by gaetz on 06/09/2024.
//

#include "Vec3.hpp"

namespace gmath {

    Vec3::Vec3() :
        x { 0 }, y { 0 }, z { 0 }
    {}

    Vec3::Vec3(f32 xP, f32 yP, f32 zP) :
        x { xP }, y { yP }, z { zP }
    {}

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
}
