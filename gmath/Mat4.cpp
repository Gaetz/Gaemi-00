//
// Created by gaetz on 24/09/2024.
//

#include "Mat4.hpp"

namespace gmath {

    const Mat4 Mat4::Identity { 1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f };

    Matrix Mat4::ToRaylib() const {
        Matrix result { m0, m1, m2, m3,
                        m4, m5, m6, m7,
                        m8, m9, m10, m11,
                        m12, m13, m14, m15 };
        return result;
    }

    Mat4 Mat4::CreateRotationMatrix(const Vec3 &axis, f32 angle) {
        Mat4 result {};
        f32 x = axis.x;
        f32 y = axis.y;
        f32 z = axis.z;
        f32 lengthSquared = x*x + y*y + z*z;

        if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f))
        {
            f32 iLength = 1.0f / sqrtf(lengthSquared);
            x *= iLength;
            y *= iLength;
            z *= iLength;
        }

        f32 sinRes = sinf(angle);
        f32 cosRes = cosf(angle);
        f32 t = 1.0f - cosRes;

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

    Mat4::Mat4() {
        m0 = 0.0f;
        m1 = 0.0f;
        m2 = 0.0f;
        m3 = 0.0f;
        m4 = 0.0f;
        m5 = 0.0f;
        m6 = 0.0f;
        m7 = 0.0f;
        m8 = 0.0f;
        m9 = 0.0f;
        m10 = 0.0f;
        m11 = 0.0f;
        m12 = 0.0f;
        m13 = 0.0f;
        m14 = 0.0f;
        m15 = 0.0f;
    }
    
    Mat4::Mat4(const Matrix& raylibMatrix) {
        m0 = raylibMatrix.m0;
        m1 = raylibMatrix.m1;
        m2 = raylibMatrix.m2;
        m3 = raylibMatrix.m3;
        m4 = raylibMatrix.m4;
        m5 = raylibMatrix.m5;
        m6 = raylibMatrix.m6;
        m7 = raylibMatrix.m7;
        m8 = raylibMatrix.m8;
        m9 = raylibMatrix.m9;
        m10 = raylibMatrix.m10;
        m11 = raylibMatrix.m11;
        m12 = raylibMatrix.m12;
        m13 = raylibMatrix.m13;
        m14 = raylibMatrix.m14;
        m15 = raylibMatrix.m15;
    }
}