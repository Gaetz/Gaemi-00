//
// Created by gaetz on 26/09/2024.
//

#ifndef GDRAW_ROUNDED_CUBE3D_HPP
#define GDRAW_ROUNDED_CUBE3D_HPP

#include <Model3D.hpp>

#include "Defines.hpp"
#include "Vec3.hpp"

using gmath::Vec3;

namespace gdraw {
    class RoundedCube3D : public gassets::Model3D {
    public:
        RoundedCube3D(i32 xSize_, i32 ySize_, i32 zSize_, f32 roundness_, Vec3 position_);

        void Draw() const;

    private:
    public:
        i32 GetXSize() const;

        i32 GetYSize() const;

        i32 GetZSize() const;

        f32 GetRoundness() const;

        void SetRoundness(f32 roundness);

    private:
        i32 xSize { 1 };
        i32 ySize { 1 };
        i32 zSize { 1 };
        f32 roundness { 2 };

        void Build();
        void Rebuild();

        i32 SetQuadIndices(vector<i32> &indices, i32 i, i32 v00, i32 v10, i32 v01, i32 v11);

        i32 CreateTopFace(vector<i32> &indices, i32 t, i32 ring);

        i32 CreateBottomFace(vector<i32> &indices, i32 vertexCount, i32 t, i32 ring);

        void SetVertex(vector<Vec3> &vertices, vector<Vec3> &normals, i32 i, i32 x, i32 y, i32 z);
    };
}

#endif //GDRAW_ROUNDED_CUBE3D_HPP
