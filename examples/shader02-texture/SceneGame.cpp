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
    AssetsManager::LoadTexture("frame", "frame.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("mask", "mask.png", ToSceneId(SceneName::SceneGame));

    AssetsManager::LoadFragmentShader("shader", "shader.frag");

    Texture& frame = AssetsManager::GetTexture("frame");
    Texture& mask = AssetsManager::GetTexture("mask");

    gdraw::SetTextureFilter(frame, gdraw::TextureFilterMode::Point);

    gdraw::GenerateTextureMipmaps(mask);
    gdraw::SetTextureWrap(mask, gdraw::TextureWrapMode::Repeat);
    gdraw::SetTextureFilter(mask, gdraw::TextureFilterMode::Trilinear);

    shaderTexture = AssetsManager::GenerateTexture(1024, 600, BLANK);

    // For the first example
    //shaderTexture = AssetsManager::GetTexture("frame");
}

void SceneGame::Update(f32 dt) {


}

void SceneGame::Draw() {

    /*
    // 1. Just draw the texture. texture0 uniform is already set by raylib
    gdraw::BeginShaderMode("shader");
    gdraw::DrawTexture(shaderTexture, 128, 60, WHITE);
     */

    /*
    // 2. Draw the texture with a tint uniform that changes the color
    gdraw::BeginShaderMode("shader");
    gdraw::SetShaderVec4("shader", "tint", Vec4 {0.2f, 0.8f, 0.6f, 1.0f});
    gdraw::DrawTexture(shaderTexture, 128, 60, WHITE);
     */

    // 3. Alpha blending + repeat texture + texture filter
    // Here texture0 is the white background. We then mix texture1 and texture2
    gdraw::BeginShaderMode("shader");
    Texture& frame = AssetsManager::GetTexture("frame");
    Texture& mask = AssetsManager::GetTexture("mask");

    gdraw::SetShaderSampler2D("shader", "texture1", frame);
    gdraw::SetShaderSampler2D("shader", "texture2", mask);
    gdraw::SetTextureWrap(mask, gdraw::TextureWrapMode::Repeat);
    gdraw::SetTextureFilter(frame, gdraw::TextureFilterMode::Point);
    gdraw::SetTextureFilter(mask, gdraw::TextureFilterMode::Trilinear);
    gdraw::DrawTexture(shaderTexture, 128, 60, WHITE);

    gdraw::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}