//
// Created by gaetz on 07/02/2024.
//

#include "SceneGame.hpp"

#include <AssetsManager.hpp>
#include "Renderer.hpp"

using gassets::AssetsManager;


SceneGame::SceneGame(Game &game) : game{game}
{


}

void SceneGame::Load() {
    AssetsManager::LoadFragmentShader("shader", "shader.frag");
    DisableCursor();
    //shaderTexture = AssetsManager::GenerateTexture(720, 720, BLANK);
}

void SceneGame::Update(f32 dt) {
    totalTime += dt;
    gdraw::SetShaderFloat("shader", "time", totalTime);

    camera.Update(dt);
}

void SceneGame::Draw() {
    gdraw::BeginMode3D(camera);

    grid.Draw();

    gdraw::EndMode3D();


//    gdraw::BeginShaderMode("shader");
//    gdraw::SetShaderVec2("shader", "resolution", Vec2(720, 720));
//
//    gdraw::DrawTexture(shaderTexture, 256, 0, WHITE);
//    gdraw::EndShaderMode();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}