//
// Created by gaetz on 07/02/2024.
//

#include "SceneGame.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "Query.hpp"

using gassets::AssetsManager;

SceneGame::SceneGame(Game& game) : game { game } {}

void SceneGame::Load() {
    AssetsManager::LoadShader("shader-lighting", "shader-lighting.vert", "shader-lighting.frag");
    AssetsManager::LoadModel("suzanne", "suzanne.glb", ToSceneId(SceneName::SceneGame));
    model = AssetsManager::GetModel("suzanne");
    model.SetMaterialShader(0, AssetsManager::GetShader("shader-lighting"));
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;

    Vec3 cameraNewPos { camera.GetPosition() };
    cameraNewPos.x += sinf(totalTime) * 3.0f * dt;
    cameraNewPos.z += cos(totalTime) * 3.0f * dt;
    camera.SetPosition(cameraNewPos);
}

void SceneGame::Draw() {
    gdraw::BeginMode3D(camera);
    const Vec3 camPosition { camera.GetPosition() };
    gdraw::SetShaderVec3("shader-lighting", "camPosition", camPosition);

    gdraw::DrawModel(model, Vec3{ 0.0f, 0.0f, 0.0f }, 0.5f, WHITE);
    gdraw::DrawGrid(10, 1.0f);
    gdraw::EndMode3D();
}

void SceneGame::Unload() {
    //AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
    AssetsManager::UnloadSceneModels(ToSceneId(SceneName::SceneGame));
}
