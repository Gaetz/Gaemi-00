//
// Created by gaetz on 24/09/2024.
//

#include "Grid3D.hpp"

#include <Log.hpp>

#include "Renderer.hpp"

Grid3D::Grid3D(i32 width_, i32 height_, bool isCentered_): width(width_), height(height_), isCentered { isCentered_ } {
    vertices.reserve((width + 1) * (height + 1));
    indices.reserve(width * height * 6);
    Generate();
}

void Grid3D::Generate() {
    const f32 widthF = static_cast<f32>(width);
    const f32 heightF = static_cast<f32>(height);
    const f32 horizontalOffset = isCentered ? -widthF / 2.0f : 0.0f;
    const f32 verticalOffset = isCentered ? -heightF / 2.0f : 0.0f;
    // Generate vertices
    for (i32 y = 0; y <= height; y++) {
        for (i32 x = 0; x <= width; x++) {
            vertices.emplace_back(x + horizontalOffset, y + verticalOffset, 0.0f);
        }
    }
    // Generate indices
    indices.resize(width * height * 6);
    for (int ti = 0, vi = 0, y = 0; y < height; y++, vi++) {
        for (int x = 0; x < width; x++, ti += 6, vi++) {
            indices[ti] = vi;
            indices[ti + 3] = indices[ti + 2] = vi + 1;
            indices[ti + 4] = indices[ti + 1] = vi + width + 1;
            indices[ti + 5] = vi + width + 2;
        }
    }
}

void Grid3D::Draw() {
    for (const auto& vertex : vertices) {
        render::DrawPoint3D(vertex + Vec3(0, 0, -0.1), RED);
    }
    for (i32 i = 0; i < indices.size(); i += 3) {
        render::DrawTriangle3D(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]], WHITE);
    }
}
