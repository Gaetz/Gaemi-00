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

    enum class CameraMode {
        Custom = CAMERA_CUSTOM,
        Free = CAMERA_FREE,
        FirstPerson = CAMERA_FIRST_PERSON,
        ThirdPerson = CAMERA_THIRD_PERSON,
        Orbital = CAMERA_ORBITAL,
    };

    constexpr f32 CAMERA_ORBITAL_SPEED { 0.5f };
    constexpr f32 CAMERA_MOVE_SPEED { 0.09f };
    constexpr f32 CAMERA_ROTATION_SPEED { 0.03f };
    constexpr f32 CAMERA_PAN_SPEED { 0.2f };
    constexpr f32 CAMERA_MOUSE_MOVE_SENSITIVITY { 0.003f };

    class Cam3D {
    public:
        Cam3D();

        explicit Cam3D(Vec3 position_, Vec3 target_ = Vec3::zero);


        void Update(f32 dt);

        Vec3 GetForward() const;
        Vec3 GetRight() const;
        void CameraMoveForward(float distance, bool moveInWorldPlane);
        void CameraMoveUp(float distance);
        void CameraMoveRight(float distance, bool moveInWorldPlane);
        void CameraMoveToTarget(float delta);
        void CameraYaw(f32 angle, bool rotateAroundTarget);
        void CameraPitch(f32 angle, bool lockView, bool rotateAroundTarget, bool rotateUp);
        void CameraRoll(f32 angle);

        ::Camera ToRaylib() const;
        ::Camera *ToRaylibPtr();

    private:


        /** Camera position */
        Vec3 position { 0.0f, 1.0f, 10.0f };

        /** Camera target it looks-at */
        Vec3 target { 0.0f, 0.0f, 0.0f };

        /** Camera up vector (rotation over its axis) */
        Vec3 up { 0.0f, 1.0f, 0.0f };

        /** Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic */
        f32 fovY { 45.0f };

        /** Projection kind */
        CameraProjection projection { CameraProjection::Perspective };

        /** Projection kind */
        CameraMode mode { CameraMode::Free };

        /** Raylib camera for sending a pointer */
        Camera3D raylibCamera {};

        void UpdateCameraFromRaylib();


    public:
        const Vec3 &GetPosition() const {
            return position;
        }

        void SetPosition(const Vec3 &position_) {
            position = position_;
        }

        const Vec3 &GetTarget() const {
            return target;
        }

        void SetTarget(const Vec3 &target_) {
            target = target_;
        }

        const Vec3 &GetUp() const {
            return up;
        }

        void SetUp(const Vec3 &up_) {
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
