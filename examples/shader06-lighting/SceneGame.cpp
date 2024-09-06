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
    AssetsManager::LoadShader("shader-ambient", "shader-ambient.vs", "shader-ambient.fs");
    model = LoadModel("assets/shader06-lighting/models/suzanne.glb");
    camera.position = { 2.0f, 1.5f, 3.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    model.materials[0].shader = AssetsManager::GetShader("shader-ambient");
    //Image imBlank = GenImageColor(600, 600, BLANK);
    //shaderTexture = LoadTextureFromImage(imBlank);
    //UnloadImage(imBlank);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;

}

void SceneGame::Draw() {
    BeginMode3D(camera);        // Begin 3d mode drawing
    DrawModel(model, { 0.0f, 0.0f, 0.0f }, 0.5f, WHITE);   // Draw 3d model with texture
    DrawGrid(10, 1.0f);     // Draw a grid
    EndMode3D();                // End 3d mode d

    //render::BeginShaderMode("shader-ambient");
    //render::SetShaderFloat("shader-ambient", "time", totalTime);

    //render::DrawTexture(shaderTexture, 128, 60, WHITE);
    //render::EndShaderMode();
}

void SceneGame::Unload() {
    //AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}