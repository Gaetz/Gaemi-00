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
    shaderTexture = AssetsManager::GenerateTexture(1080, 720, BLANK);

    AssetsManager::LoadTexture("tomato", "tomato.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("vignette", "vignette.png", ToSceneId(SceneName::SceneGame));

    Texture& tomatoTex = AssetsManager::GetTexture("tomato");
    Texture& vignetteTex = AssetsManager::GetTexture("vignette");

    gdraw::GenerateTextureMipmaps(tomatoTex);
    gdraw::SetTextureWrap(tomatoTex, gdraw::TextureWrapMode::Repeat);
    gdraw::SetTextureFilter(tomatoTex, gdraw::TextureFilterMode::Bilinear);

    gdraw::GenerateTextureMipmaps(vignetteTex);
    gdraw::SetTextureWrap(vignetteTex, gdraw::TextureWrapMode::Repeat);
    gdraw::SetTextureFilter(vignetteTex, gdraw::TextureFilterMode::Bilinear);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;
    gdraw::SetShaderFloat("shader", "time", totalTime);
}

void SceneGame::Draw() {
    gdraw::BeginShaderMode("shader");
    gdraw::SetShaderVec2("shader", "resolution", Vec2(1080, 720));

    Texture& tomatoTex = AssetsManager::GetTexture("tomato");
    Texture& vignetteTex = AssetsManager::GetTexture("vignette");
    gdraw::SetShaderSampler2D("shader", "texture1", tomatoTex);
    gdraw::SetShaderSampler2D("shader", "texture2", vignetteTex);

    gdraw::DrawTexture(shaderTexture, 100, 0, WHITE);
    gdraw::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}
