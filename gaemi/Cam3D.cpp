//
// Created by gaetz on 11/09/2024.
//

#include "Cam3D.hpp"

namespace render {
    ::Camera Cam3D::ToRaylib() const {
        return { position.ToRaylib(), target.ToRaylib(), up.ToRaylib(), fovY, static_cast<i32>(projection) };
    }
} // render