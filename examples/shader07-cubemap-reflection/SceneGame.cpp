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

    model = AssetsManager::GetModel("suzanne");


    skyboxCube = AssetsManager::GenerateCube(1.0f, 1.0f, 1.0f);
    skyboxCube.materials[0].shader = AssetsManager::GetShader("shader-skybox");
    model.materials[0].shader = AssetsManager::GetShader("shader-lighting");

    AssetsManager::LoadTextureCubemap("cubemap_sky", "cubemap_sky.png", CubemapTextureLayout::Vertical, ToSceneId(SceneName::SceneGame));
    skyboxTexture = AssetsManager::GetTexture("cubemap_sky");

    render::SetShaderCubemapOnModel(skyboxCube, "shader-skybox", "environmentMap", skyboxTexture);
    render::SetShaderCubemapOnModel(model, "shader-lighting", "environmentMap", skyboxTexture);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;

    Vec3 cameraNewPos { camera.GetPosition() };
    cameraNewPos.x += sinf(totalTime) * 3.0f * dt;
    cameraNewPos.z += cos(totalTime) * 3.0f * dt;
    camera.SetPosition(cameraNewPos);
}

void SceneGame::Draw() {
    render::BeginMode3D(camera);
    Vec3 camPosition { camera.GetPosition() };
    render::SetShaderVec3("shader-lighting", "camPosition", camPosition);
    //render::SetShaderSamplerCube("shader-lighting", "skybox", skyboxTexture_);

    // We are inside the cube, we need to disable backface culling!
    render::DisableBackfaceCulling();
    render::DisableDepthMask();
    DrawModel(skyboxCube, { 0, 0, 0}, 1.0f, WHITE);
    render::EnableBackfaceCulling();
    render::EnableDepthMask();

    DrawModel(model, { 0.0f, 0.0f, 0.0f }, 0.5f, WHITE);
    DrawGrid(10, 1.0f);
    render::EndMode3D();
}

void SceneGame::Unload() {
    //AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
    AssetsManager::UnloadSceneModels(ToSceneId(SceneName::SceneGame));
}