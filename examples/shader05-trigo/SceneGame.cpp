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
    AssetsManager::LoadFragmentShader("shader", "shader.frag");

    shaderTexture = AssetsManager::GenerateTexture(600, 600, BLANK);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;

}

void SceneGame::Draw() {
    render::BeginShaderMode("shader");
    render::SetShaderFloat("shader", "time", totalTime);

    render::DrawTexture(shaderTexture, 128, 60, WHITE);
    render::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}