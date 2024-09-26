//
// Created by gaetz on 26/09/2024.
//

#include "Cube3D.hpp"

#include <Log.hpp>

#include "Renderer.hpp"

namespace gdraw
{
    Cube3D::Cube3D(i32 xSize_, i32 ySize_, i32 zSize_, Vec3 position_):
        xSize { xSize_ }, ySize { ySize_ },
        zSize { zSize_ }, position { position_ } {
        i32 cornerVertices { 8 };
        i32 edgeVertices { (xSize + ySize + zSize - 3) * 4 };
        i32 faceVertices { ((xSize - 1) * (ySize - 1) + (xSize - 1) * (zSize - 1) + (ySize - 1) * (zSize - 1)) * 2 };
        const i32 vertexCount { cornerVertices + edgeVertices + faceVertices };
        vertices.reserve(vertexCount);
        vertices.resize(vertexCount);
        i32 quadsCount = (xSize * ySize + xSize * zSize + ySize * zSize) * 2;
        indices.reserve(quadsCount * 6);
        indices.resize(quadsCount * 6);
        Generate();
    }

    void Cube3D::Generate() {
        // Generate vertices
        i32 v { 0 };
        // -- Lateral faces
        for (i32 y = 0; y <= ySize; y++) {
            for (i32 x = 0; x <= xSize; x++) {
                vertices[v++] = position + Vec3(x, y, 0);
            }
            for (i32 z = 1; z <= xSize; z++) {
                vertices[v++] = position + Vec3(xSize, y, z);
            }
            for (i32 x = xSize - 1; x >= 0; x--) {
                vertices[v++] = position + Vec3(x, y, zSize);
            }
            for (i32 z = zSize - 1; z > 0; z--) {
                vertices[v++] = position + Vec3(0, y, z);
            }
        }
        // -- Top and bottom
        for (i32 z = 1; z < zSize; z++) {
            for (i32 x = 1; x < xSize; x++) {
                vertices[v++] = position + Vec3(x, ySize, z);
            }
        }
        for (i32 z = 1; z < zSize; z++) {
            for (i32 x = 1; x < xSize; x++) {
                vertices[v++] = position + Vec3(x, 0, z);
            }
        }
        // Generate indices
        i32 ring { (xSize + zSize) * 2 };
        i32 t { 0 };
        v = 0;
        // -- Lateral faces
        for (i32 y = 0; y < ySize; y++, v++) {
            for (i32 q = 0; q < ring - 1; q++, v++) {
                t = SetQuadIndices(t, v, v + 1, v + ring, v + ring + 1);
            }
            t = SetQuadIndices(t, v, v - ring + 1, v + ring, v + 1);
        }
        // -- Top and bottom
        t = CreateTopFace(t, ring);
        t = CreateBottomFace(t, ring);
    }

    i32 Cube3D::SetQuadIndices(i32 i, i32 v00, i32 v10, i32 v01, i32 v11) {
        indices[i] = v00;
        indices[i + 1] = indices[i + 4] = v01;
        indices[i + 2] = indices[i + 3] = v10;
        indices[i + 5] = v11;
        return i + 6;
    }

    i32 Cube3D::CreateTopFace(i32 t, i32 ring) {
        i32 v = ring * ySize;
        for (i32 x = 0; x < xSize - 1; x++, v++) {
            t = SetQuadIndices(t, v, v + 1, v + ring - 1, v + ring);
        }
        t = SetQuadIndices(t, v, v + 1, v + ring - 1, v + 2);

        i32 vMin = ring * (ySize + 1) - 1;
        i32 vMid = vMin + 1;
        i32 vMax = v + 2;

        for (i32 z = 1; z < zSize - 1; z++, vMin--, vMid++, vMax++) {
            t = SetQuadIndices(t, vMin, vMid, vMin - 1, vMid + xSize - 1);
            for (i32 x = 1; x < xSize - 1; x++, vMid++) {
                t = SetQuadIndices(t, vMid, vMid + 1, vMid + xSize - 1, vMid + xSize);
            }
            t = SetQuadIndices(t, vMid, vMax, vMid + xSize - 1, vMax + 1);
        }

        i32 vTop = vMin - 2;
        t = SetQuadIndices(t, vMin, vMid, vTop + 1, vTop);
        for (i32 x = 1; x < xSize - 1; x++, vTop--, vMid++) {
            t = SetQuadIndices(t, vMid, vMid + 1, vTop, vTop - 1);
        }
        t = SetQuadIndices(t, vMid, vTop - 2, vTop, vTop - 1);

        return t;
    }

    i32 Cube3D::CreateBottomFace(i32 t, i32 ring) {
        i32 v = 1;
        i32 vMid = vertices.size() - (xSize - 1) * (zSize - 1);
        t = SetQuadIndices(t, ring - 1, vMid, 0, 1);
        for (i32 x = 1; x < xSize - 1; x++, v++, vMid++) {
            t = SetQuadIndices(t, vMid, vMid + 1, v, v + 1);
        }
        t = SetQuadIndices(t, vMid, v + 2, v, v + 1);

        i32 vMin = ring - 2;
        vMid -= xSize - 2;
        i32 vMax = v + 2;

        for (i32 z = 1; z < zSize - 1; z++, vMin--, vMid++, vMax++) {
            t = SetQuadIndices(t, vMin, vMid + xSize - 1, vMin + 1, vMid);
            for (i32 x = 1; x < xSize - 1; x++, vMid++) {
                t = SetQuadIndices(t, vMid + xSize - 1, vMid + xSize, vMid, vMid + 1);
            }
            t = SetQuadIndices(t, vMid + xSize - 1, vMax + 1, vMid, vMax);
        }

        i32 vTop = vMin - 1;
        t = SetQuadIndices(t, vTop + 1, vTop, vTop + 2, vMid);
        for (i32 x = 1; x < xSize - 1; x++, vTop--, vMid++) {
            t = SetQuadIndices(t, vTop, vTop - 1, vMid, vMid + 1);
        }
        t = SetQuadIndices(t, vTop, vTop - 1, vMid, vTop - 2);

        return t;
    }

    void Cube3D::Draw() {
        for (const auto& vertex : vertices) {
            gdraw::DrawPoint3D(vertex + Vec3(0, 0, -0.05), RED);
        }
        for (i32 i = 0; i < indices.size(); i += 3) {
            gdraw::DrawTriangle3D(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]], WHITE);
        }
    }
}
