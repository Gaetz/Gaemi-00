//
// Created by gaetz on 11/09/2024.
//

#include "Cam3D.hpp"
#include "Mat4.hpp"

using gmath::Mat4;

namespace render {

    Cam3D::Cam3D() {
        raylibCamera.position = position.ToRaylib();
        raylibCamera.target = target.ToRaylib();
        raylibCamera.up = up.ToRaylib();
        raylibCamera.fovy = fovY;
        raylibCamera.projection = static_cast<i32>(projection);
    }

    Cam3D::Cam3D(Vec3 position_, Vec3 target_) : position(position_), target(target_) {
        raylibCamera.position = position.ToRaylib();
        raylibCamera.target = target.ToRaylib();
        raylibCamera.up = up.ToRaylib();
        raylibCamera.fovy = fovY;
        raylibCamera.projection = static_cast<i32>(projection);
    }

    void Cam3D::Update(f32 dt) {
        Vector2 mousePositionDelta = GetMouseDelta();

        bool moveInWorldPlane = ((mode == CameraMode::FirstPerson) || (mode == CameraMode::ThirdPerson));
        bool rotateAroundTarget = ((mode == CameraMode::ThirdPerson) || (mode == CameraMode::Orbital));
        bool lockView = ((mode == CameraMode::FirstPerson) || (mode == CameraMode::ThirdPerson) ||
                         (mode == CameraMode::Orbital));
        bool rotateUp = false;

        if (mode == CameraMode::Orbital) {
            // Orbital can just orbit
            const Mat4 rotation = Mat4::CreateRotationMatrix(up.Normalize(), CAMERA_ORBITAL_SPEED * dt);
            Vec3 view = position - target;
            view = rotation * view;
            position = target + view;
        } else {
            // Camera rotation
            if (IsKeyDown(KEY_DOWN))
                CameraPitch(-CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
            if (IsKeyDown(KEY_UP)) CameraPitch(CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
            if (IsKeyDown(KEY_RIGHT)) CameraYaw(-CAMERA_ROTATION_SPEED, rotateAroundTarget);
            if (IsKeyDown(KEY_LEFT)) CameraYaw(CAMERA_ROTATION_SPEED, rotateAroundTarget);
            if (IsKeyDown(KEY_Z)) CameraRoll(-CAMERA_ROTATION_SPEED);
            if (IsKeyDown(KEY_C)) CameraRoll(CAMERA_ROTATION_SPEED);

            // Camera movement
            if (!IsGamepadAvailable(0)) {
                // Camera pan (for CAMERA_FREE)
                if ((mode == CameraMode::Free) && (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))) {
                    const Vector2 mouseDelta = GetMouseDelta();
                    if (mouseDelta.x > 0.0f) CameraMoveRight(CAMERA_PAN_SPEED, moveInWorldPlane);
                    if (mouseDelta.x < 0.0f) CameraMoveRight(-CAMERA_PAN_SPEED, moveInWorldPlane);
                    if (mouseDelta.y > 0.0f) CameraMoveUp(-CAMERA_PAN_SPEED);
                    if (mouseDelta.y < 0.0f) CameraMoveUp(CAMERA_PAN_SPEED);
                } else {
                    // Mouse support
                    CameraYaw(-mousePositionDelta.x * CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
                    CameraPitch(-mousePositionDelta.y * CAMERA_MOUSE_MOVE_SENSITIVITY, lockView,
                                rotateAroundTarget, rotateUp);
                }

                // Keyboard support
                if (IsKeyDown(KEY_W)) CameraMoveForward(CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (IsKeyDown(KEY_A)) CameraMoveRight(-CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (IsKeyDown(KEY_S)) CameraMoveForward(-CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (IsKeyDown(KEY_D)) CameraMoveRight(CAMERA_MOVE_SPEED, moveInWorldPlane);
            } else {
                // Gamepad controller support
                CameraYaw(
                        -(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X) * 2) * CAMERA_MOUSE_MOVE_SENSITIVITY,
                        rotateAroundTarget);
                CameraPitch(
                        -(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y) * 2) * CAMERA_MOUSE_MOVE_SENSITIVITY,
                        lockView, rotateAroundTarget, rotateUp);

                if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) <= -0.25f)
                    CameraMoveForward(CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) <= -0.25f)
                    CameraMoveRight(-CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) >= 0.25f)
                    CameraMoveForward(-CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) >= 0.25f)
                    CameraMoveRight(CAMERA_MOVE_SPEED, moveInWorldPlane);
            }

            if (mode == CameraMode::Free) {
                if (IsKeyDown(KEY_Q)) CameraMoveUp(CAMERA_MOVE_SPEED);
                if (IsKeyDown(KEY_E)) CameraMoveUp(-CAMERA_MOVE_SPEED);
            }
        }

        if ((mode == CameraMode::ThirdPerson) || (mode == CameraMode::Orbital) || (mode == CameraMode::Free)) {
            // Zoom target distance
            CameraMoveToTarget(-GetMouseWheelMove());
            if (IsKeyPressed(KEY_KP_SUBTRACT)) CameraMoveToTarget(2.0f);
            if (IsKeyPressed(KEY_KP_ADD)) CameraMoveToTarget(-2.0f);
        }

        //UpdateCamera(&raylibCamera, CAMERA_FREE);
        //UpdateCameraFromRaylib();
    }

    // Moves the camera in its forward direction
    void Cam3D::CameraMoveForward(float distance, bool moveInWorldPlane) {
        Vec3 forwardMove = GetForward();

        if (moveInWorldPlane) {
            // Project vector onto world plane
            forwardMove.y = 0;
            forwardMove.Normalize();
        }

        // Scale by distance
        forwardMove *= distance;

        // Move position and target
        position += forwardMove;
        target += forwardMove;
    }

    // Moves the camera in its up direction
    void Cam3D::CameraMoveUp(float distance) {
        Vec3 upMove = up;

        // Scale by distance
        upMove *= distance;

        // Move position and target
        position += upMove;
        target += upMove;
    }

    // Moves the camera target in its current right direction
    void Cam3D::CameraMoveRight(float distance, bool moveInWorldPlane) {
        Vec3 rightMove = GetRight();

        if (moveInWorldPlane) {
            // Project vector onto world plane
            rightMove.y = 0;
            rightMove.Normalize();
        }

        // Scale by distance
        rightMove *= distance;

        // Move position and target
        position += rightMove;
        target += rightMove;
    }

    // Moves the camera position closer/farther to/from the camera target
    void Cam3D::CameraMoveToTarget(float delta) {
        float distance = Vec3::Distance(position, target);

        // Apply delta
        distance += delta;

        // Distance must be greater than 0
        if (distance <= 0) distance = 0.001f;

        // Set new distance by moving the position along the forward vector
        Vec3 forward = GetForward();
        position = target + (forward * -distance);
    }

    // Rotates the camera around its up vector
    // Yaw is "looking left and right"
    // If rotateAroundTarget is false, the camera rotates around its position
    // Note: angle must be provided in radians
    void Cam3D::CameraYaw(f32 angle, bool rotateAroundTarget) {
        // View vector
        Vec3 targetPosition = target - position;

        // Rotate view vector around up axis
        targetPosition.RotateByAxisAngle(up, angle);

        if (rotateAroundTarget) {
            // Move position relative to target
            position = target - targetPosition;
        } else { // rotate around camera.position
            // Move target relative to position
            target = position + targetPosition;
        }
    }

    // Rotates the camera around its right vector, pitch is "looking up and down"
    //  - lockView prevents camera overrotation (aka "somersaults")
    //  - rotateAroundTarget defines if rotation is around target or around its position
    //  - rotateUp rotates the up direction as well (typically only usefull in CAMERA_FREE)
    // NOTE: angle must be provided in radians
    void Cam3D::CameraPitch(f32 angle, bool lockView, bool rotateAroundTarget, bool rotateUp) {
        // View vector
        Vec3 targetPosition = target - position;

        if (lockView) {
            // In these camera modes we clamp the Pitch angle
            // to allow only viewing straight up or down.

            // Clamp view up
            float maxAngleUp = Vec3::Angle(up, targetPosition);
            maxAngleUp -= 0.001f; // avoid numerical errors
            if (angle > maxAngleUp) angle = maxAngleUp;

            // Clamp view down
            float maxAngleDown = Vec3::Angle(-1.0f * up, targetPosition);
            maxAngleDown *= -1.0f; // downwards angle is negative
            maxAngleDown += 0.001f; // avoid numerical errors
            if (angle < maxAngleDown) angle = maxAngleDown;
        }

        // Rotation axis
        Vec3 right = GetRight();

        // Rotate view vector around right axis
        targetPosition.RotateByAxisAngle(right, angle);

        if (rotateAroundTarget) {
            // Move position relative to target
            position = target - targetPosition;
        } else { // rotate around camera.position

            // Move target relative to position
            target = position + targetPosition;
        }

        if (rotateUp) {
            // Rotate up direction around right axis
            up.RotateByAxisAngle(right, angle);
        }
    }

    // Rotates the camera around its forward vector
    // Roll is "turning your head sideways to the left or right"
    // Note: angle must be provided in radians
    void Cam3D::CameraRoll(f32 angle) {
        // Rotation axis
        Vec3 forward = GetForward();

        // Rotate up direction around forward axis
        up.RotateByAxisAngle(forward, angle);
    }

    ::Camera Cam3D::ToRaylib() const {
        return {
                position.ToRaylib(),
                target.ToRaylib(),
                up.ToRaylib(),
                fovY,
                static_cast<i32>(projection)
        };
    }

    ::Camera *Cam3D::ToRaylibPtr() {
        return &raylibCamera;
    }

    void Cam3D::UpdateCameraFromRaylib() {
        position = Vec3::FromRaylib(raylibCamera.position);
        target = Vec3::FromRaylib(raylibCamera.target);
        up = Vec3::FromRaylib(raylibCamera.up);
        fovY = raylibCamera.fovy;
        projection = static_cast<CameraProjection>(raylibCamera.projection);
    }

    Vec3 Cam3D::GetForward() const {
        return (target - position).Normalize();
    }

    Vec3 Cam3D::GetRight() const {
        return Vec3::Cross(up, GetForward()).Normalize();
    }
} // render