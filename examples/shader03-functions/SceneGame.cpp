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
    AssetsManager::LoadFragmentShader("shader", "shader.fs");

    Image imBlank = GenImageColor(600, 600, BLANK);
    shaderTexture = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);
}

void SceneGame::Update(f32 dt) {


}

void SceneGame::Draw() {
    render::BeginShaderMode("shader");
    render::SetShaderVec4("shader", "color1", Vec4(1, 0, 0, 1));
    render::SetShaderVec4("shader", "color2", Vec4(0, 0, 1, 1));

    render::DrawTexture(shaderTexture, 128, 60, WHITE);
    render::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}