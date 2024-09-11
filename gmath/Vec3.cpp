//
// Created by gaetz on 06/09/2024.
//

#include "Vec3.hpp"

namespace gmath {

    Vec3::Vec3() :
        x { 0 }, y { 0 }, z { 0 }
    {}

    Vec3::Vec3(f32 x_, f32 y_, f32 z_) :
            x { x_ }, y { y_ }, z { z_ }
    {}

    const Vec3 Vec3::zero {0.0f, 0.0f , 0.0f};
    const Vec3 Vec3::up {0.0f, 1.0f , 0.0f};
    const Vec3 Vec3::right {1.0f, 0.0f , 0.0f};
    const Vec3 Vec3::front {0.0f, 0.0f , 1.0f};

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
