//
// Created by gaetz on 26/09/2024.
//

#include "SphereCube3D.hpp"

#include <Log.hpp>
#include <Mat4.hpp>
#include <Renderer.hpp>

using gmath::Mat4;

namespace gdraw {
    SphereCube3D::SphereCube3D(i32 gridSize_, f32 radius_, Vec3 position_) :
            gridSize { gridSize_ },
            radius { radius_ } {

        // Build graphics data
        Build();

        transform = Mat4::Identity;
        transform.m3 = position_.x;
        transform.m7 = position_.y;
        transform.m11 = position_.z;

        // Setup materials
        materialCount = 1;
        materials = new Material[materialCount];
        materials[0] = LoadMaterialDefault();
        meshMaterial = new int[meshCount];
        meshMaterial[0] = 0; // First material index
    }

    void SphereCube3D::Build() {
        // Setup geometry data
        i32 cornerVertices { 8 };
        i32 edgeVertices { (gridSize + gridSize + gridSize - 3) * 4 };
        i32 faceVertices { ((gridSize - 1) * (gridSize - 1) + (gridSize - 1) * (gridSize - 1) + (gridSize - 1) * (gridSize - 1)) * 2 };
        const i32 vertexCount { cornerVertices + edgeVertices + faceVertices };
        i32 quadsCount = (gridSize * gridSize + gridSize * gridSize + gridSize * gridSize) * 2;

        vector<Vec3> vertices;
        vector<Vec3> normals;
        vector<i32> indices;
        vertices.reserve(vertexCount);
        vertices.resize(vertexCount);
        normals.reserve(vertexCount);
        normals.resize(vertexCount);
        indices.reserve(quadsCount * 6);
        indices.resize(quadsCount * 6);

        // Generate vertices
        i32 v { 0 };
        // -- Lateral faces
        for (i32 y = 0; y <= gridSize; y++) {
            for (i32 x = 0; x <= gridSize; x++) {
                SetVertex(vertices, normals, v++, x, y, 0);
            }
            for (i32 z = 1; z <= gridSize; z++) {
                SetVertex(vertices, normals, v++, gridSize, y, z);
            }
            for (i32 x = gridSize - 1; x >= 0; x--) {
                SetVertex(vertices, normals, v++, x, y, gridSize);
            }
            for (i32 z = gridSize - 1; z > 0; z--) {
                SetVertex(vertices, normals, v++, 0, y, z);
            }
        }
        // -- Top and bottom
        for (i32 z = 1; z < gridSize; z++) {
            for (i32 x = 1; x < gridSize; x++) {
                SetVertex(vertices, normals, v++, x, gridSize, z);
            }
        }
        for (i32 z = 1; z < gridSize; z++) {
            for (i32 x = 1; x < gridSize; x++) {
                SetVertex(vertices, normals, v++, x, 0, z);
            }
        }

        // Generate indices
        i32 ring { (gridSize + gridSize) * 2 };
        i32 t { 0 };
        v = 0;
        // -- Lateral faces
        for (i32 y = 0; y < gridSize; y++, v++) {
            for (i32 q = 0; q < ring - 1; q++, v++) {
                t = SetQuadIndices(indices, t, v, v + 1, v + ring, v + ring + 1);
            }
            t = SetQuadIndices(indices, t, v, v - ring + 1, v + ring, v + 1);
        }
        // -- Top and bottom
        t = CreateTopFace(indices, t, ring);
        t = CreateBottomFace(indices, vertexCount, t, ring);

        // Create mesh
        meshCount = 1;
        meshes = new Mesh[meshCount];
        meshes[0].vaoId = 0;
        meshes[0].vboId = nullptr;
        meshes[0].vertexCount = vertexCount;
        meshes[0].triangleCount = quadsCount * 2;
        meshes[0].vertices = new f32[vertexCount * 3];
        meshes[0].normals = new f32[vertexCount * 3];
        meshes[0].indices = new u16[quadsCount * 6];
        meshes[0].texcoords = new f32[vertexCount * 2];

        meshes[0].texcoords2 = nullptr;
        meshes[0].tangents = nullptr;
        meshes[0].animVertices = nullptr;
        meshes[0].animNormals = nullptr;
        meshes[0].boneIds = nullptr;
        meshes[0].boneWeights = nullptr;

        for (i32 i = 0; i < vertexCount; ++i) {
            meshes[0].vertices[i * 3] = vertices[i].x;
            meshes[0].vertices[i * 3 + 1] = vertices[i].y;
            meshes[0].vertices[i * 3 + 2] = vertices[i].z;
            meshes[0].normals[i * 3] = normals[i].x;
            meshes[0].normals[i * 3 + 1] = normals[i].y;
            meshes[0].normals[i * 3 + 2] = normals[i].z;
        }
        for (i32 i = 0; i < quadsCount * 6; ++i) {
            meshes[0].indices[i] = indices[i];
        }
        for (i32 i = 0; i < vertexCount * 2; ++i) {
            meshes[0].texcoords[i] = 0;
        }
        meshes[0].colors = new u8[vertexCount * 4];
        for (i32 i = 0; i < vertexCount * 4; ++i) {
            meshes[0].colors[i] = 255;
        }

        // Send mesh to GPU
        UploadMesh(meshes, false);
    }

    void SphereCube3D::Rebuild() {
        // Save Material and Mesh data
        i32 formerMaterialCount = materialCount;
        i32 formerMeshCount = meshCount;
        Material formerMaterials = *materials;
        i32 formerMeshMaterials = *meshMaterial;
        Shader formerShader = materials[0].shader;

        // Unload previous mesh data
        Unload();

        // Setup materials again
        materialCount = formerMaterialCount;
        materials = new Material[materialCount];
        materials[0] = formerMaterials;
        meshCount = formerMeshCount;
        meshMaterial = new int[meshCount];
        meshMaterial[0] = formerMeshMaterials;
        SetMaterialShader(0, formerShader);

        // Build again
        Build();
    }

    i32 SphereCube3D::SetQuadIndices(vector<i32> &indices, i32 i, i32 v00, i32 v10, i32 v01, i32 v11) {
        indices[i] = v00;
        indices[i + 1] = indices[i + 4] = v01;
        indices[i + 2] = indices[i + 3] = v10;
        indices[i + 5] = v11;
        return i + 6;
    }

    i32 SphereCube3D::CreateTopFace(vector<i32> &indices, i32 t, i32 ring) {
        i32 v = ring * gridSize;
        for (i32 x = 0; x < gridSize - 1; x++, v++) {
            t = SetQuadIndices(indices, t, v, v + 1, v + ring - 1, v + ring);
        }
        t = SetQuadIndices(indices, t, v, v + 1, v + ring - 1, v + 2);

        i32 vMin = ring * (gridSize + 1) - 1;
        i32 vMid = vMin + 1;
        i32 vMax = v + 2;

        for (i32 z = 1; z < gridSize - 1; z++, vMin--, vMid++, vMax++) {
            t = SetQuadIndices(indices, t, vMin, vMid, vMin - 1, vMid + gridSize - 1);
            for (i32 x = 1; x < gridSize - 1; x++, vMid++) {
                t = SetQuadIndices(indices, t, vMid, vMid + 1, vMid + gridSize - 1, vMid + gridSize);
            }
            t = SetQuadIndices(indices, t, vMid, vMax, vMid + gridSize - 1, vMax + 1);
        }

        i32 vTop = vMin - 2;
        t = SetQuadIndices(indices, t, vMin, vMid, vTop + 1, vTop);
        for (i32 x = 1; x < gridSize - 1; x++, vTop--, vMid++) {
            t = SetQuadIndices(indices, t, vMid, vMid + 1, vTop, vTop - 1);
        }
        t = SetQuadIndices(indices, t, vMid, vTop - 2, vTop, vTop - 1);

        return t;
    }

    i32 SphereCube3D::CreateBottomFace(vector<i32> &indices, i32 vertexCount, i32 t, i32 ring) {
        i32 v = 1;
        i32 vMid = vertexCount - (gridSize - 1) * (gridSize - 1);
        t = SetQuadIndices(indices, t, ring - 1, vMid, 0, 1);
        for (i32 x = 1; x < gridSize - 1; x++, v++, vMid++) {
            t = SetQuadIndices(indices, t, vMid, vMid + 1, v, v + 1);
        }
        t = SetQuadIndices(indices, t, vMid, v + 2, v, v + 1);

        i32 vMin = ring - 2;
        vMid -= gridSize - 2;
        i32 vMax = v + 2;

        for (i32 z = 1; z < gridSize - 1; z++, vMin--, vMid++, vMax++) {
            t = SetQuadIndices(indices, t, vMin, vMid + gridSize - 1, vMin + 1, vMid);
            for (i32 x = 1; x < gridSize - 1; x++, vMid++) {
                t = SetQuadIndices(indices, t, vMid + gridSize - 1, vMid + gridSize, vMid, vMid + 1);
            }
            t = SetQuadIndices(indices, t, vMid + gridSize - 1, vMax + 1, vMid, vMax);
        }

        i32 vTop = vMin - 1;
        t = SetQuadIndices(indices, t, vTop + 1, vTop, vTop + 2, vMid);
        for (i32 x = 1; x < gridSize - 1; x++, vTop--, vMid++) {
            t = SetQuadIndices(indices, t, vTop, vTop - 1, vMid, vMid + 1);
        }
        t = SetQuadIndices(indices, t, vTop, vTop - 1, vMid, vTop - 2);

        return t;
    }

    void SphereCube3D::SetVertex(vector<Vec3> &vertices, vector<Vec3> &normals, i32 i, i32 x, i32 y, i32 z) {
        Vec3 v = Vec3 { static_cast<f32>(x), static_cast<f32>(y), static_cast<f32>(z) } * 2.0f / gridSize - Vec3::one;
        normals[i] = v.Normalize();
        vertices[i] = normals[i] * radius;
    }

    void SphereCube3D::Draw() const {
        Vec3 position { transform.m12, transform.m13, transform.m14 };
        DrawModel(this, position, 1.0f, WHITE);
    }
}
