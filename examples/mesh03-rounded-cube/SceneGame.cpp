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
    DisableCursor();
    AssetsManager::LoadShader("shader", "shader.vert", "shader.frag");

}

void SceneGame::Update(f32 dt) {
    camera.Update(dt);
}

void SceneGame::Draw() {
    gdraw::BeginMode3D(camera);

    //gdraw::BeginShaderMode("shader");
    cube.Draw();
    //gdraw::EndShaderMode();

    gdraw::EndMode3D();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}