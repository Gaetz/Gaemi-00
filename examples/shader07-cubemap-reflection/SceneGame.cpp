//
// Created by gaetz on 07/02/2024.
//

#include "SceneGame.hpp"
#include <iomanip>
#include "ImRenderer.h"
#include "../gecs/World.hpp"
#include "../gecs/Entity.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "Query.hpp"
#include "rlgl.h"

using gecs::Position;
using gecs::Velocity;
using gecs::Sprite;
using gecs::Query;

SceneGame::SceneGame(Game &game) : game{game}
{


}

void SceneGame::Load() {
    AssetsManager::LoadShader("shader-lighting", "shader-lighting.vs", "shader-lighting.fs");
    AssetsManager::LoadShader("shader-skybox", "shader-skybox.vs", "shader-skybox.fs");
    AssetsManager::LoadModel("suzanne", "suzanne.glb", ToSceneId(SceneName::SceneGame));

    model_ = AssetsManager::GetModel("suzanne");
    camera_.position = { -3.0f, 0.4f, -1.0f };
    camera_.target = { 0.0f, 0.0f, 0.0f };
    camera_.up = { 0.0f, 1.0f, 0.0f };
    camera_.fovy = 45.0f;
    camera_.projection = CAMERA_PERSPECTIVE;


    skyboxCube_ = AssetsManager::GenerateCube(1.0f, 1.0f, 1.0f);
    skyboxCube_.materials[0].shader = AssetsManager::GetShader("shader-skybox");
    model_.materials[0].shader = AssetsManager::GetShader("shader-lighting");

    AssetsManager::LoadTextureCubemap("cubemap_sky", "cubemap_sky.png", CubemapTextureLayout::Vertical, ToSceneId(SceneName::SceneGame));
    skyboxTexture_ = AssetsManager::GetTexture("cubemap_sky");

    render::SetShaderCubemapOnModel(skyboxCube_, "shader-skybox", "environmentMap", skyboxTexture_);
    render::SetShaderCubemapOnModel(model_, "shader-lighting", "environmentMap", skyboxTexture_);
}

void SceneGame::Update(f32 dt) {
    totalTime_ += dt;

    camera_.position.x += sinf(totalTime_) * 3.0f * dt;
    camera_.position.z += cos(totalTime_) * 3.0f * dt;
}

void SceneGame::Draw() {
    BeginMode3D(camera_);
    Vec3 camPosition { camera_.position.x, camera_.position.y, camera_.position.z };
    render::SetShaderVec3("shader-lighting", "camPosition", camPosition);
    //render::SetShaderSamplerCube("shader-lighting", "skybox", skyboxTexture_);

    // We are inside the cube, we need to disable backface culling!
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(skyboxCube_, {0, 0, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();

    DrawModel(model_, { 0.0f, 0.0f, 0.0f }, 0.5f, WHITE);
    DrawGrid(10, 1.0f);
    EndMode3D();
}

void SceneGame::Unload() {
    //AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
    AssetsManager::UnloadSceneModels(ToSceneId(SceneName::SceneGame));
}