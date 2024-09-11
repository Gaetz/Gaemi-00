//
// Created by gaetz on 11/09/2024.
//

#ifndef GAEMI_CAM3D_HPP
#define GAEMI_CAM3D_HPP

#include "Defines.hpp"
#include "Vec3.hpp"

using gmath::Vec3;

namespace render {

    enum class CameraProjection {
        Perspective = CAMERA_PERSPECTIVE,
        Orthographic = CAMERA_ORTHOGRAPHIC
    };

    class Cam3D {
    public:
        explicit Cam3D(Vec3 position_, Vec3 target_ = Vec3::up)
        : position { position_ }, target { target_ }
        {}

        ::Camera ToRaylib() const;

    private:
        /** Camera position */
        Vec3 position { 0.0f, 1.0f, 5.0f };

        /** Camera target it looks-at */
        Vec3 target { 0.0f, 0.0f, 0.0f };

        /** Camera up vector (rotation over its axis) */
        Vec3 up { 0.0f, 1.0f, 0.0f };

        /** Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic */
        f32 fovY { 45.0f };

        /** Projection kind */
        CameraProjection projection { CameraProjection::Perspective };

    public:
        const Vec3& GetPosition() const {
            return position;
        }

        void SetPosition(const Vec3& position_) {
            position = position_;
        }

        const Vec3& GetTarget() const {
            return target;
        }

        void SetTarget(const Vec3& target_) {
            target = target_;
        }

        const Vec3& GetUpVector() const {
            return up;
        }

        void SetUpVector(const Vec3& up_) {
            up = up_;
        }

        f32 GetFovY() const {
            return fovY;
        }

        void SetFovY(f32 fovY_) {
            fovY = fovY_;
        }

        CameraProjection GetProjection() const {
            return projection;
        }

        void SetProjection(CameraProjection projection_) {
            projection = projection_;
        }

    };

} // render

#endif //GAEMI_CAM3D_HPP
