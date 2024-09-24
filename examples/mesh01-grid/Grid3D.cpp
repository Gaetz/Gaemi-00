//
// Created by gaetz on 24/09/2024.
//

#include "Grid3D.hpp"
#include "Renderer.hpp"

Grid3D::Grid3D(i32 width_, i32 height_, bool isCentered_): width(width_), height(height_), isCentered { isCentered_ } {
    vertices.reserve(width * height);
    Generate();
}

void Grid3D::Generate() {
    const f32 widthF = static_cast<f32>(width);
    const f32 heightF = static_cast<f32>(height);
    const f32 horizontalOffset = isCentered ? -widthF / 2.0f : 0.0f;
    const f32 verticalOffset = isCentered ? -heightF / 2.0f : 0.0f;
    for (i32 y = 0; y < height; y++) {
        for (i32 x = 0; x < width; x++) {
            vertices.emplace_back(static_cast<f32>(x) + horizontalOffset, static_cast<f32>(y) + verticalOffset, 0);
        }
    }
}

void Grid3D::Draw() {
    render::DrawTriangleStrip3D(vertices, WHITE);
}
