//
// Created by gaetz on 07/02/2024.
//

#include "SceneGame.hpp"
#include <iomanip>
#include "ImRenderer.h"
#include "../ghecs/World.hpp"
#include "../ghecs/Entity.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "Query.hpp"

using ghecs::Position;
using ghecs::Velocity;
using ghecs::Sprite;
using ghecs::Query;

SceneGame::SceneGame(Game& game) : game { game } {}

void SceneGame::Load() {
    AssetsManager::LoadShader("shader-lighting", "shader-lighting.vert", "shader-lighting.frag");
    AssetsManager::LoadShader("shader-skybox", "shader-skybox.vert", "shader-skybox.frag");
    AssetsManager::LoadModel("suzanne", "suzanne.glb", ToSceneId(SceneName::SceneGame));

    // 1.2. Vertex manipulation with a model
//    model = AssetsManager::GetModel("suzanne");
    // 3. Ripple effect with a sphere
    model = AssetsManager::GenerateSphere(1.0f, 64, 64);

    model.SetMaterialShader(0, AssetsManager::GetShader("shader-lighting"));

    skyboxCube = AssetsManager::GenerateCube(1.0f, 1.0f, 1.0f);
    skyboxCube.SetMaterialShader(0, AssetsManager::GetShader("shader-skybox"));

    AssetsManager::LoadTextureCubemap("cubemap_sky", "cubemap_sky.png", gassets::CubemapTextureLayout::Vertical,
                                      ToSceneId(SceneName::SceneGame));
    skyboxTexture = AssetsManager::GetTexture("cubemap_sky");
    gdraw::SetShaderCubemapOnModel(skyboxCube, "shader-skybox", "environmentMap", skyboxTexture);
    gdraw::SetShaderCubemapOnModel(model, "shader-lighting", "environmentMap", skyboxTexture);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;
    gdraw::SetShaderFloat("shader-lighting", "time", totalTime);

}

void SceneGame::Draw() {
    gdraw::BeginMode3D(camera);
    const Vec3 camPosition { camera.GetPosition() };
    gdraw::SetShaderVec3("shader-lighting", "camPosition", camPosition);

    // We are inside the cube, we need to disable backface culling!
    gdraw::DisableBackfaceCulling();
    gdraw::DisableDepthMask();
    gdraw::DrawModel(skyboxCube, Vec3 { 0, 0, 0 }, 1.0f, WHITE);
    gdraw::EnableBackfaceCulling();
    gdraw::EnableDepthMask();

    gdraw::DrawModel(model, Vec3 { 0.0f, 0.0f, 0.0f }, 0.5f, WHITE);
    gdraw::DrawGrid(10, 1.0f);
    gdraw::EndMode3D();
}

void SceneGame::Unload() {
    //AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
    AssetsManager::UnloadSceneModels(ToSceneId(SceneName::SceneGame));
}
