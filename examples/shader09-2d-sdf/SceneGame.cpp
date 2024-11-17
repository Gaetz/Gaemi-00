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

using gassets::AssetsManager;

SceneGame::SceneGame(Game &game) : game{game}
{


}

void SceneGame::Load() {
    AssetsManager::LoadFragmentShader("shader", "shader.frag");
    AssetsManager::LoadFragmentShader("cloudy", "cloudy.frag");

    shaderTexture = AssetsManager::GenerateTexture(720, 720, BLANK);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;
    gdraw::SetShaderFloat("shader", "time", totalTime);
    gdraw::SetShaderFloat("cloudy", "time", totalTime);
}

void SceneGame::Draw() {
    gdraw::BeginShaderMode("shader");
    gdraw::SetShaderVec2("shader", "resolution", Vec2(720, 720));
    //gdraw::BeginShaderMode("cloudy");
    //gdraw::SetShaderVec2("cloudy", "resolution", Vec2(720, 720));

    gdraw::DrawTexture(shaderTexture, 256, 0, WHITE);
    gdraw::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}