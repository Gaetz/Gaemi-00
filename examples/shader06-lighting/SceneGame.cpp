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
    model = LoadModel("assets/shader06-lighting/models/suzanne.glb");
    camera.position = { 2.0f, 1.5f, 3.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    model.materials[0].shader = AssetsManager::GetShader("shader-lighting");
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;

}

void SceneGame::Draw() {
    BeginMode3D(camera);
    Vec3 camPosition { camera.position.x, camera.position.y, camera.position.z };
    render::SetShaderVec3("shader-lighting", "camPosition", camPosition);
    DrawModel(model, { 0.0f, 0.0f, 0.0f }, 0.5f, WHITE);
    DrawGrid(10, 1.0f);
    EndMode3D();
}

void SceneGame::Unload() {
    //AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}