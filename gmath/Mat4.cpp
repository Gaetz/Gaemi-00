//
// Created by gaetz on 24/09/2024.
//

#include "Mat4.hpp"

namespace gmath {

    Mat4 Mat4::CreateRotationMatrix(const Vec3 &axis, float angle) {
        Mat4 result { 0 };

        float x = axis.x, y = axis.y, z = axis.z;

        float lengthSquared = x*x + y*y + z*z;

        if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f))
        {
            float iLength = 1.0f / sqrtf(lengthSquared);
            x *= iLength;
            y *= iLength;
            z *= iLength;
        }

        float sinRes = sinf(angle);
        float cosRes = cosf(angle);
        float t = 1.0f - cosRes;

        result.m0 = x*x*t + cosRes;
        result.m1 = y*x*t + z * sinRes;
        result.m2 = z*x*t - y * sinRes;
        result.m3 = 0.0f;

        result.m4 = x*y*t - z * sinRes;
        result.m5 = y*y*t + cosRes;
        result.m6 = z*y*t + x * sinRes;
        result.m7 = 0.0f;

        result.m8 = x*z*t + y * sinRes;
        result.m9 = y*z*t - x * sinRes;
        result.m10 = z*z*t + cosRes;
        result.m11 = 0.0f;

        result.m12 = 0.0f;
        result.m13 = 0.0f;
        result.m14 = 0.0f;
        result.m15 = 1.0f;

        return result;
    }
}