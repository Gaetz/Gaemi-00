//
// Created by gaetz on 07/02/2024.
//

#include "SceneGame.hpp"
#include "ImRenderer.h"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "Query.hpp"

using gassets::AssetsManager;

SceneGame::SceneGame(Game& game) : game { game } {}

void SceneGame::Load() {
    AssetsManager::LoadFragmentShader("shader", "shader.frag");

    shaderTexture = AssetsManager::GenerateTexture(720, 720, BLANK);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;
    gdraw::SetShaderFloat("shader", "time", totalTime);
}

void SceneGame::Draw() {
    gdraw::BeginShaderMode("shader");
    gdraw::SetShaderVec2("shader", "resolution", Vec2(720, 720));

    gdraw::DrawTexture(shaderTexture, 256, 0, WHITE);
    gdraw::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}
