//
// Created by gaetz on 11/09/2024.
//

#include "Cam3D.hpp"
#include "Mat4.hpp"

using gmath::Mat4;

namespace gdraw {
    Cam3D::Cam3D(Vec3 position_, Vec3 target_) : position(position_), target(target_) {}

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
        }
        else {
            // Camera rotation
            if (IsKeyDown(KEY_DOWN))
                Pitch(-CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
            if (IsKeyDown(KEY_UP))
                Pitch(CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
            if (IsKeyDown(KEY_RIGHT))
                Yaw(-CAMERA_ROTATION_SPEED, rotateAroundTarget);
            if (IsKeyDown(KEY_LEFT))
                Yaw(CAMERA_ROTATION_SPEED, rotateAroundTarget);
            if (IsKeyDown(KEY_Z))
                Roll(-CAMERA_ROTATION_SPEED);
            if (IsKeyDown(KEY_C))
                Roll(CAMERA_ROTATION_SPEED);

            // Camera movement
            if (!IsGamepadAvailable(0)) {
                // Camera pan (for CAMERA_FREE)
                if ((mode == CameraMode::Free) && (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))) {
                    const Vector2 mouseDelta = GetMouseDelta();
                    if (mouseDelta.x > 0.0f)
                        MoveRight(-CAMERA_PAN_SPEED, moveInWorldPlane);
                    if (mouseDelta.x < 0.0f)
                        MoveRight(CAMERA_PAN_SPEED, moveInWorldPlane);
                    if (mouseDelta.y > 0.0f)
                        MoveUp(-CAMERA_PAN_SPEED);
                    if (mouseDelta.y < 0.0f)
                        MoveUp(CAMERA_PAN_SPEED);
                }
                else {
                    // Mouse support
                    Yaw(-mousePositionDelta.x * CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
                    Pitch(-mousePositionDelta.y * CAMERA_MOUSE_MOVE_SENSITIVITY, lockView,
                          rotateAroundTarget, rotateUp);
                }

                // Keyboard support
                if (IsKeyDown(KEY_W))
                    MoveForward(CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (IsKeyDown(KEY_A))
                    MoveRight(CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (IsKeyDown(KEY_S))
                    MoveForward(-CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (IsKeyDown(KEY_D))
                    MoveRight(-CAMERA_MOVE_SPEED, moveInWorldPlane);
            }
            else {
                // Gamepad controller support
                Yaw(
                    -(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X) * 2) * CAMERA_MOUSE_MOVE_SENSITIVITY,
                    rotateAroundTarget);
                Pitch(
                    -(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y) * 2) * CAMERA_MOUSE_MOVE_SENSITIVITY,
                    lockView, rotateAroundTarget, rotateUp);

                if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) <= -0.25f)
                    MoveForward(CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) <= -0.25f)
                    MoveRight(-CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) >= 0.25f)
                    MoveForward(-CAMERA_MOVE_SPEED, moveInWorldPlane);
                if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) >= 0.25f)
                    MoveRight(CAMERA_MOVE_SPEED, moveInWorldPlane);
            }

            if (mode == CameraMode::Free) {
                if (IsKeyDown(KEY_Q))
                    MoveUp(-CAMERA_MOVE_SPEED);
                if (IsKeyDown(KEY_E))
                    MoveUp(CAMERA_MOVE_SPEED);
            }
        }

        if ((mode == CameraMode::ThirdPerson) || (mode == CameraMode::Orbital) || (mode == CameraMode::Free)) {
            // Zoom target distance
            MoveToTarget(-GetMouseWheelMove());
            if (IsKeyPressed(KEY_KP_SUBTRACT))
                MoveToTarget(2.0f);
            if (IsKeyPressed(KEY_KP_ADD))
                MoveToTarget(-2.0f);
        }
    }

    // Moves the camera in its forward direction
    void Cam3D::MoveForward(float distance, bool moveInWorldPlane) {
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
    void Cam3D::MoveUp(float distance) {
        Vec3 upMove = up;

        // Scale by distance
        upMove *= distance;

        // Move position and target
        position += upMove;
        target += upMove;
    }

    // Moves the camera target in its current right direction
    void Cam3D::MoveRight(float distance, bool moveInWorldPlane) {
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
    void Cam3D::MoveToTarget(float delta) {
        float distance = Vec3::Distance(position, target);

        // Apply delta
        distance += delta;

        // Distance must be greater than 0
        if (distance <= 0)
            distance = 0.001f;

        // Set new distance by moving the position along the forward vector
        Vec3 forward = GetForward();
        position = target + (forward * -distance);
    }

    // Rotates the camera around its up vector
    // Yaw is "looking left and right"
    // If rotateAroundTarget is false, the camera rotates around its position
    // Note: angle must be provided in radians
    void Cam3D::Yaw(f32 angle, bool rotateAroundTarget) {
        // View vector
        Vec3 targetPosition = target - position;

        // Rotate view vector around up axis
        targetPosition.RotateByAxisAngle(up, angle);

        if (rotateAroundTarget) {
            // Move position relative to target
            position = target - targetPosition;
        }
        else {
            // rotate around camera.position
            // Move target relative to position
            target = position + targetPosition;
        }
    }

    // Rotates the camera around its right vector, pitch is "looking up and down"
    //  - lockView prevents camera overrotation (aka "somersaults")
    //  - rotateAroundTarget defines if rotation is around target or around its position
    //  - rotateUp rotates the up direction as well (typically only usefull in CAMERA_FREE)
    // NOTE: angle must be provided in radians
    void Cam3D::Pitch(f32 angle, bool lockView, bool rotateAroundTarget, bool rotateUp) {
        // View vector
        Vec3 targetPosition = target - position;

        if (lockView) {
            // In these camera modes we clamp the Pitch angle
            // to allow only viewing straight up or down.

            // Clamp view up
            float maxAngleUp = Vec3::Angle(up, targetPosition);
            maxAngleUp -= 0.001f; // avoid numerical errors
            if (angle > maxAngleUp)
                angle = maxAngleUp;

            // Clamp view down
            float maxAngleDown = Vec3::Angle(-1.0f * up, targetPosition);
            maxAngleDown *= -1.0f; // downwards angle is negative
            maxAngleDown += 0.001f; // avoid numerical errors
            if (angle < maxAngleDown)
                angle = maxAngleDown;
        }

        // Rotation axis
        Vec3 right = GetRight();

        // Rotate view vector around right axis
        targetPosition.RotateByAxisAngle(right, angle);

        if (rotateAroundTarget) {
            // Move position relative to target
            position = target - targetPosition;
        }
        else {
            // rotate around camera.position

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
    void Cam3D::Roll(f32 angle) {
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

    Vec3 Cam3D::GetForward() const {
        return (target - position).Normalize();
    }

    Vec3 Cam3D::GetRight() const {
        return Vec3::Cross(up, GetForward()).Normalize();
    }
} // render
