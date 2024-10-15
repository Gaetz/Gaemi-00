//
// Created by Gaetz on 12/09/2024.
//

#ifndef GASSETS_MODEL3D_HPP
#define GASSETS_MODEL3D_HPP

#include "Defines.hpp"
#include "raylib.h"
#include "../gmath/Mat4.hpp"

using gmath::Mat4;

namespace gassets {
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
    };;

    class Model3D {
    public:
        Model3D() = default;

        ~Model3D() = default;

        explicit Model3D(const Model &raylibModel_);

        explicit Model3D(Model &&raylibModel_);

        ::Model ToRaylib() const {
            return { transform.ToRaylib(), meshCount, materialCount, meshes, materials, meshMaterial, boneCount, bones, bindPose };
        }

        void SetTransform(const Mat4 &transform_);

        void SetMaterialShader(i32 material_, const Shader &shader_);

        void SetMaterialMapTexture(i32 material_, MaterialMapType mapType_, const Texture2D &texture_);

        void Unload() const;

    protected:
        /** Local transform matrix */
        Mat4 transform { Mat4::Identity };

        /** Number of meshes */
        i32 meshCount { 0 };
        /** Number of materials */
        i32 materialCount { 0 };
        /** Meshes array */
        Mesh *meshes { nullptr };
        /** Materials array */
        Material *materials { nullptr };
        /** Mesh material number */
        i32 *meshMaterial { nullptr };

        // Animation data

        /** Number of bones */
        i32 boneCount { 0 };
        /** Bones information (skeleton) */
        BoneInfo *bones { nullptr };
        /** Bones base transformation (pose) */
        Transform *bindPose { nullptr };
    };
}
#endif //GASSETS_MODEL3D_HPP
