//
// Created by gaetz on 07/02/2024.
//

#include "SceneGame.hpp"
#include <iomanip>
#include "ImRenderer.h"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "Query.hpp"

using gassets::AssetsManager;

SceneGame::SceneGame(Game &game) : game{game}
{


}

void SceneGame::Load() {
    AssetsManager::LoadFragmentShader("shader", "shader.frag");

    shaderTexture = AssetsManager::GenerateTexture(600, 600, BLANK);
}

void SceneGame::Update(f32 dt) {


}

void SceneGame::Draw() {
    gdraw::BeginShaderMode("shader");
    gdraw::SetShaderVec2("shader", "resolution", Vec2(600, 600));

    gdraw::DrawTexture(shaderTexture, 128, 60, WHITE);
    gdraw::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}