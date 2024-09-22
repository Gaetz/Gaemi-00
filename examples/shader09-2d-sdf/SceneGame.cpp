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

    shaderTexture = AssetsManager::GenerateTexture(720, 720, BLANK);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;
    render::SetShaderFloat("shader", "time", totalTime);
}

void SceneGame::Draw() {
    render::BeginShaderMode("shader");
    render::SetShaderVec2("shader", "resolution", Vec2(720, 720));

    render::DrawTexture(shaderTexture, 256, 0, WHITE);
    render::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}