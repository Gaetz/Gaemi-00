//
// Created by Gaetz on 12/09/2024.
//

#ifndef GAEMI_MODEL3D_HPP
#define GAEMI_MODEL3D_HPP

#include "Defines.hpp"
#include "raylib.h"

namespace render {

enum class MaterialMapType {
    Albedo = MATERIAL_MAP_ALBEDO,
    Metalness = MATERIAL_MAP_METALNESS,
    Normal = MATERIAL_MAP_NORMAL,
    Roughness = MATERIAL_MAP_ROUGHNESS,
    Occlusion = MATERIAL_MAP_OCCLUSION,
    Emission = MATERIAL_MAP_EMISSION,
    Height = MATERIAL_MAP_HEIGHT,
    Cubemap = MATERIAL_MAP_CUBEMAP,
    Irradiance = MATERIAL_MAP_IRRADIANCE,
    Prefilter = MATERIAL_MAP_PREFILTER,
    BRDF = MATERIAL_MAP_BRDF
};
;

class Model3D {
public:
    Model3D() = default;
    ~Model3D() = default;
    explicit Model3D(const Model& raylibModel_);
    explicit Model3D(Model&& raylibModel_);

    ::Model ToRaylib() const {
        return { transform, meshCount, materialCount, meshes, materials, meshMaterial, boneCount, bones, bindPose };
    }

    void SetMaterialShader(i32 material_, const Shader& shader_);
    void SetMaterialMapTexture(i32 material_, MaterialMapType mapType_, const Texture2D& texture_);

    void Unload() const;

private:
    /** Local transform matrix */
    Matrix transform;

    /** Number of meshes */
    i32 meshCount;
    /** Number of materials */
    i32 materialCount;
    /** Meshes array */
    Mesh *meshes;
    /** Materials array */
    Material *materials;
    /** Mesh material number */
    i32 *meshMaterial;

    // Animation data

    /** Number of bones */
    i32 boneCount;
    /** Bones information (skeleton) */
    BoneInfo *bones;
    /** Bones base transformation (pose) */
    Transform *bindPose;
};

}


#endif //GAEMI_MODEL3D_HPP
