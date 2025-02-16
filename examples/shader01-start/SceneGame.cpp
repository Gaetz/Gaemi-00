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
using gassets::AssetsManager;

SceneGame::SceneGame(Game &game) : game{game}
{


}

void SceneGame::Load() {
    AssetsManager::LoadFragmentShader("shader", "shader.frag");
    shaderTexture = AssetsManager::GenerateTexture(1024, 600, BLANK);
}

void SceneGame::Update(f32 dt) {


}

void SceneGame::Draw() {
    gdraw::BeginShaderMode("shader");
    gdraw::SetShaderVec4("shader", "color1", Vec4(1, 0, 0, 1));
    gdraw::SetShaderVec4("shader", "color2", Vec4(0, 0, 1, 1));

    gdraw::DrawTexture(shaderTexture, 128, 60, WHITE);
    gdraw::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}