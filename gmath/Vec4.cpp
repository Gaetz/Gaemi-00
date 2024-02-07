//
// Created by gaetz on 07/02/2024.
//

#include "Vec4.hpp"

namespace gmath {

    Vec4::Vec4() :
        x { 0 }, y { 0 }, z { 0 }, w { 0 }
    {}

    Vec4::Vec4(f32 xP, f32 yP, f32 zP, f32 wP) :
        x { xP }, y { yP }, z { zP }, w { wP }
    {}

    Vec4& Vec4::operator+=(const Vec4& r) {
        x += r.x;
        y += r.y;
        z += r.z;
        w += r.w;
        return *this;
    }

    Vec4& Vec4::operator-=(const Vec4& r) {
        x -= r.x;
        y -= r.y;
        z -= r.z;
        w -= r.w;
        return *this;
    }

    Vec4& Vec4::operator*=(f32 scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    Vec4& Vec4::operator/=(const f32 scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    str Vec4::Log() const {
        return "{ x: " + std::to_string(x) + ", y:" + std::to_string(y) +
            ", z:" + std::to_string(z) + ", w:" + std::to_string(w) + " }";
    }
}
