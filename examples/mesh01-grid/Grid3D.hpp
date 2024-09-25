//
// Created by gaetz on 24/09/2024.
//

#ifndef MESH01_GRID3D_HPP
#define MESH01_GRID3D_HPP

#include "Defines.hpp"
#include "Vec3.hpp"

using gmath::Vec3;

class Grid3D {
public:
    Grid3D(i32 width_, i32 height_, bool isCentered_);
    void Draw();

private:
    i32 width { 0 };
    i32 height { 0 };
    bool isCentered { false };
    vector<Vec3> vertices;
    vector<i32> indices;

    void Generate();
};

#endif //MESH01_GRID3D_HPP
