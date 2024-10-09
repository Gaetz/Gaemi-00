//
// Created by gaetz on 26/09/2024.
//

#include "RoundedCube3D.hpp"

#include <Log.hpp>

#include "Renderer.hpp"

namespace gdraw {
    RoundedCube3D::RoundedCube3D(i32 xSize_, i32 ySize_, i32 zSize_, f32 roundness_, Vec3 position_) :
            xSize { xSize_ }, ySize { ySize_ },
            zSize { zSize_ }, roundness { roundness_ }, position { position_ } {
        i32 cornerVertices { 8 };
        i32 edgeVertices { (xSize + ySize + zSize - 3) * 4 };
        i32 faceVertices { ((xSize - 1) * (ySize - 1) + (xSize - 1) * (zSize - 1) + (ySize - 1) * (zSize - 1)) * 2 };
        vertexCount = cornerVertices + edgeVertices + faceVertices;
        i32 quadsCount = (xSize * ySize + xSize * zSize + ySize * zSize) * 2;
        Generate(vertexCount, quadsCount);
    }

    /*
    RoundedCube3D::RoundedCube3D(i32 xSize_, i32 ySize_, i32 zSize_, f32 roundness_, Vec3 position_):
        xSize { xSize_ }, ySize { ySize_ },
        zSize { zSize_ }, roundness { roundness_ }, position { position_ } {
        i32 cornerVertices { 8 };
        i32 edgeVertices { (xSize + ySize + zSize - 3) * 4 };
        i32 faceVertices { ((xSize - 1) * (ySize - 1) + (xSize - 1) * (zSize - 1) + (ySize - 1) * (zSize - 1)) * 2 };
        const i32 vertexCount { cornerVertices + edgeVertices + faceVertices };
        vertices.reserve(vertexCount);
        vertices.resize(vertexCount);
        normals.reserve(vertexCount);
        normals.resize(vertexCount);
        i32 quadsCount = (xSize * ySize + xSize * zSize + ySize * zSize) * 2;
        indices.reserve(quadsCount * 6);
        indices.resize(quadsCount * 6);
        Generate();
    }
    */

    void RoundedCube3D::Generate(i32 vertexCount, i32 quadsCount) {
        vertices = (f32 *) malloc(vertexCount * sizeof(f32) * 3);
        normals = (f32 *) malloc(vertexCount * sizeof(f32) * 3);
        indices = (u16 *) malloc(quadsCount * 6 * sizeof(u16));
        // Generate vertices
        i32 v { 0 };
        // -- Lateral faces
        for (i32 y = 0; y <= ySize; y++) {
            for (i32 x = 0; x <= xSize; x++) {
                SetVertex(v++, x, y, 0);
            }
            for (i32 z = 1; z <= xSize; z++) {
                SetVertex(v++, xSize, y, z);
            }
            for (i32 x = xSize - 1; x >= 0; x--) {
                SetVertex(v++, x, y, zSize);
            }
            for (i32 z = zSize - 1; z > 0; z--) {
                SetVertex(v++, 0, y, z);
            }
        }
        // -- Top and bottom
        for (i32 z = 1; z < zSize; z++) {
            for (i32 x = 1; x < xSize; x++) {
                SetVertex(v++, x, ySize, z);
            }
        }
        for (i32 z = 1; z < zSize; z++) {
            for (i32 x = 1; x < xSize; x++) {
                SetVertex(v++, x, 0, z);
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

    i32 RoundedCube3D::SetQuadIndices(i32 i, i32 v00, i32 v10, i32 v01, i32 v11) {
        indices[i] = v00;
        indices[i + 1] = indices[i + 4] = v01;
        indices[i + 2] = indices[i + 3] = v10;
        indices[i + 5] = v11;
        return i + 6;
    }

    i32 RoundedCube3D::CreateTopFace(i32 t, i32 ring) {
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

    i32 RoundedCube3D::CreateBottomFace(i32 t, i32 ring) {
        i32 v = 1;
        //  i32 vMid = vertices.size() - (xSize - 1) * (zSize - 1);
        i32 vMid = vertexCount - (xSize - 1) * (zSize - 1);
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

    void RoundedCube3D::SetVertex(i32 i, f32 x, f32 y, f32 z) {
        vertices[i] = x;
        vertices[i + 1] = y;
        vertices[i + 2] = z;
        Vec3 inner { x, y, z };
        Vec3 vertexData { x, y, z };

        if (x < roundness) {
            inner.x = roundness;
        } else if (x > xSize - roundness) {
            inner.x = xSize - roundness;
        }
        if (y < roundness) {
            inner.y = roundness;
        } else if (y > ySize - roundness) {
            inner.y = ySize - roundness;
        }
        if (z < roundness) {
            inner.z = roundness;
        } else if (z > zSize - roundness) {
            inner.z = zSize - roundness;
        }

        Vec3 normalData = (vertexData - inner).Normalize();
        normals[i] = normalData.x;
        normals[i + 1] = normalData.y;
        normals[i + 2] = normalData.z;
        vertices[i] = inner.x + normals[i] * roundness;
        vertices[i + 1] = inner.y + normals[i + 1] * roundness;
        vertices[i + 2] = inner.z + normals[i + 2] * roundness;
    }

    void RoundedCube3D::Draw() const {
        for (i32 i = 0; i < vertexCount; ++i) {
            Vec3 vertexPos { position.x + vertices[i], position.y + vertices[i + 1], position.z + vertices[i + 2] };
            gdraw::DrawPoint3D(vertexPos + Vec3(0, 0, -0.05), RED);
            //gdraw::DrawLine3D(vertexPos, vertexPos + normals[i], YELLOW);
        }
        i32 indexCount = vertexCount / 3;
        for (i32 i = 0; i < indexCount; i += 3) {
            Vec3 triangle0 { position.x + vertices[indices[i]], position.y + vertices[indices[i] + 1], position.z + vertices[indices[i] + 2] };
            Vec3 triangle1 { position.x + vertices[indices[i+1]], position.y + vertices[indices[i+1] + 1], position.z + vertices[indices[i+1] + 2] };
            Vec3 triangle2 { position.x + vertices[indices[i+2]], position.y + vertices[indices[i+2] + 1], position.z + vertices[indices[i+2] + 2] };
            gdraw::DrawTriangle3D(triangle0, triangle1, triangle2, WHITE);
        }
    }

    /*
    void RoundedCube3D::SetVertex(i32 i, i32 x, i32 y, i32 z) {
        Vec3 inner = vertices[i] = Vec3(x, y, z);

        if (x < roundness) {
            inner.x = roundness;
        }
        else if (x > xSize - roundness) {
            inner.x = xSize - roundness;
        }
        if (y < roundness) {
            inner.y = roundness;
        }
        else if (y > ySize - roundness) {
            inner.y = ySize - roundness;
        }
        if (z < roundness) {
            inner.z = roundness;
        }
        else if (z > zSize - roundness) {
            inner.z = zSize - roundness;
        }

        normals[i] = (vertices[i] - inner).Normalize();
        vertices[i] = inner + normals[i] * roundness;
    }

    void RoundedCube3D::Draw() const {
        for (i32 i = 0; i < vertices.size(); ++i) {
            Vec3 vertexPos = position + vertices[i];
            gdraw::DrawPoint3D(vertexPos + Vec3(0, 0, -0.05), RED);
            //gdraw::DrawLine3D(vertexPos, vertexPos + normals[i], YELLOW);
        }
        for (i32 i = 0; i < indices.size(); i += 3) {
            gdraw::DrawTriangle3D(position + vertices[indices[i]],
                                  position + vertices[indices[i + 1]],
                                  position + vertices[indices[i + 2]], WHITE);
        }
    }

     */
}
