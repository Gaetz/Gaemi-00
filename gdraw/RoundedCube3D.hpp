//
// Created by gaetz on 26/09/2024.
//

#ifndef GDRAW_ROUNDED_CUBE3D_HPP
#define GDRAW_ROUNDED_CUBE3D_HPP

#include "Defines.hpp"
#include "Vec3.hpp"

using gmath::Vec3;

namespace gdraw
{
    class RoundedCube3D : public Mesh
    {
    public:
        RoundedCube3D(i32 xSize_, i32 ySize_, i32 zSize_, f32 roundness_, Vec3 position_);
        void Draw() const;

    private:
        Vec3 position { 0.0f, 0.0f, 0.0f };
        i32 xSize { 1 };
        i32 ySize { 1 };
        i32 zSize { 1 };
        f32 roundness { 2 };

        void Generate(i32 vertexCount, i32 quadsCount);
        i32 SetQuadIndices(i32 i, i32 v00, i32 v10, i32 v01, i32 v11);
        i32 CreateTopFace(i32 t, i32 ring);
        i32 CreateBottomFace(i32 t, i32 ring);
        void SetVertex(i32 i, f32 x, f32 y, f32 z);
    };
}

#endif //GDRAW_ROUNDED_CUBE3D_HPP
