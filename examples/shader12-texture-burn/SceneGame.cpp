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

    AssetsManager::LoadTexture("treasure-chest", "treasure-chest.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("treasure-map", "treasure-map.png", ToSceneId(SceneName::SceneGame));

    Texture& mapTex = AssetsManager::GetTexture("treasure-map");
    Texture& chestTex = AssetsManager::GetTexture("treasure-chest");

    gdraw::GenerateTextureMipmaps(mapTex);
    gdraw::SetTextureWrap(mapTex, gdraw::TextureWrapMode::Repeat);
    gdraw::SetTextureFilter(mapTex, gdraw::TextureFilterMode::Bilinear);

    gdraw::GenerateTextureMipmaps(chestTex);
    gdraw::SetTextureWrap(chestTex, gdraw::TextureWrapMode::Repeat);
    gdraw::SetTextureFilter(chestTex, gdraw::TextureFilterMode::Bilinear);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;
    gdraw::SetShaderFloat("shader", "time", totalTime);
}

void SceneGame::Draw() {
    gdraw::BeginShaderMode("shader");
    gdraw::SetShaderVec2("shader", "resolution", Vec2(720, 720));

    Texture& mapTex = AssetsManager::GetTexture("treasure-map");
    Texture& chestTex = AssetsManager::GetTexture("treasure-chest");
    gdraw::SetShaderSampler2D("shader", "texture1", mapTex);
    gdraw::SetShaderSampler2D("shader", "texture2", chestTex);

    gdraw::DrawTexture(shaderTexture, 256, 0, WHITE);
    gdraw::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}
