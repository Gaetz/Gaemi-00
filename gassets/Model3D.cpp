//
// Created by gaetz on 12/09/2024.
//

#include "Model3D.hpp"

#include <Log.hpp>

namespace gassets {
    Model3D::Model3D(const Model &raylibModel_) :
            transform { raylibModel_.transform },
            meshCount { raylibModel_.meshCount },
            materialCount { raylibModel_.materialCount },
            meshes { raylibModel_.meshes },
            materials { raylibModel_.materials },
            meshMaterial { raylibModel_.meshMaterial },
            boneCount { raylibModel_.boneCount },
            bones { raylibModel_.bones },
            bindPose { raylibModel_.bindPose } {}

    Model3D::Model3D(Model &&raylibModel_) :
            transform { raylibModel_.transform },
            meshCount { raylibModel_.meshCount },
            materialCount { raylibModel_.materialCount },
            meshes { raylibModel_.meshes },
            materials { raylibModel_.materials },
            meshMaterial { raylibModel_.meshMaterial },
            boneCount { raylibModel_.boneCount },
            bones { raylibModel_.bones },
            bindPose { raylibModel_.bindPose } {}

    void Model3D::Unload() const {
        // Unload meshes
        for (int i = 0; i < meshCount; i++)
            UnloadMesh(meshes[i]);

        // Unload materials maps
        // NOTE: As the user could be sharing shaders and textures between models,
        // we don't unload the material but just free its maps,
        // the user is responsible for freeing models shaders and textures
        for (int i = 0; i < materialCount; i++)
            delete materials[i].maps;

        // Unload arrays
        delete meshes;
        delete materials;
        delete meshMaterial;

        // Unload animation data
        delete bones;
        delete bindPose;

        LOG(LogLevel::Info) << "MODEL: Unloaded model (and meshes) from RAM and VRAM";
    }

    void Model3D::SetMaterialShader(i32 material_, const Shader &shader_) {
        materials[material_].shader = shader_;
    }

    void Model3D::SetMaterialMapTexture(i32 material_, MaterialMapType mapType_, const Texture2D &texture_) {
        materials[material_].maps[static_cast<i32>(mapType_)].texture = texture_;
    }

    void Model3D::SetTransform(const Mat4 &transform_) {
        transform = transform_;
    }
}
