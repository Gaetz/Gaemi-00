//
// Created by gaetz on 17/10/2024.
//

#ifndef GDRAW_SPHERE_CUBE3D_HPP
#define GDRAW_SPHERE_CUBE3D_HPP

#include <Model3D.hpp>

#include "Defines.hpp"
#include "Vec3.hpp"

using gmath::Vec3;

namespace gdraw {
    class SphereCube3D : public gassets::Model3D {
    public:
        SphereCube3D(i32 gridSize_, f32 radius_, Vec3 position_);

        void Draw() const;

    private:
        i32 gridSize { 1 };
        f32 radius { 1.0f };

        void Build();
        void Rebuild();

        i32 SetQuadIndices(vector<i32> &indices, i32 i, i32 v00, i32 v10, i32 v01, i32 v11);

        i32 CreateTopFace(vector<i32> &indices, i32 t, i32 ring);

        i32 CreateBottomFace(vector<i32> &indices, i32 vertexCount, i32 t, i32 ring);

        void SetVertex(vector<Vec3> &vertices, vector<Vec3> &normals, i32 i, i32 x, i32 y, i32 z);
    };
}

#endif //GDRAW_SPHERE_CUBE3D_HPP
