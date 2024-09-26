//
// Created by gaetz on 26/09/2024.
//

#ifndef GDRAW_CUBE3D_HPP
#define GDRAW_CUBE3D_HPP

#include "Defines.hpp"
#include "Vec3.hpp"

using gmath::Vec3;

namespace gdraw
{
    class Cube3D
    {
    public:
        Cube3D(i32 xSize_, i32 ySize_, i32 zSize_, Vec3 position_);
        void Draw();

    private:
        Vec3 position { 0.0f, 0.0f, 0.0f };
        i32 xSize { 1 };
        i32 ySize { 1 };
        i32 zSize { 1 };
        vector<Vec3> vertices;
        vector<i32> indices;

        void Generate();
        i32 SetQuadIndices(i32 i, i32 v00, i32 v10, i32 v01, i32 v11);
        i32 CreateTopFace(i32 t, i32 ring);
        i32 CreateBottomFace(i32 t, i32 ring);
    };
}

#endif //GDRAW_CUBE3D_HPP
