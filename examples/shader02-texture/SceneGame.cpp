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
    AssetsManager::LoadTexture("frame", "frame.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("mask", "mask.png", ToSceneId(SceneName::SceneGame));

    AssetsManager::LoadFragmentShader("shader", "shader.fs");

    Texture& frame = AssetsManager::GetTexture("frame");
    Texture& mask = AssetsManager::GetTexture("mask");

    render::SetTextureFilter(frame, render::TextureFilterMode::Point);

    render::GenerateTextureMipmaps(mask);
    render::SetTextureWrap(mask, render::TextureWrapMode::Repeat);
    render::SetTextureFilter(mask, render::TextureFilterMode::Trilinear);

    shaderTexture = AssetsManager::GenerateTexture(1024, 600, BLANK);

    // For the first example
    //shaderTexture = AssetsManager::GetTexture("frame");
}

void SceneGame::Update(f32 dt) {


}

void SceneGame::Draw() {

    /*
    // 1. Just draw the texture. texture0 uniform is already set by raylib
    render::BeginShaderMode("shader");
    render::DrawTexture(shaderTexture, 128, 60, WHITE);
     */

    /*
    // 2. Draw the texture with a tint uniform that changes the color
    render::BeginShaderMode("shader");
    render::SetShaderVec4("shader", "tint", Vec4 {0.2f, 0.8f, 0.6f, 1.0f});
    render::DrawTexture(shaderTexture, 128, 60, WHITE);
     */

    // 3. Alpha blending + repeat texture + texture filter
    // Here texture0 is the white background. We then mix texture1 and texture2
    render::BeginShaderMode("shader");
    Texture& frame = AssetsManager::GetTexture("frame");
    Texture& mask = AssetsManager::GetTexture("mask");

    render::SetShaderSampler2D("shader", "texture1", frame);
    render::SetShaderSampler2D("shader", "texture2", mask);
    render::SetTextureWrap(mask, render::TextureWrapMode::Repeat);
    render::SetTextureFilter(frame, render::TextureFilterMode::Point);
    render::SetTextureFilter(mask, render::TextureFilterMode::Trilinear);
    render::DrawTexture(shaderTexture, 128, 60, WHITE);

    render::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}