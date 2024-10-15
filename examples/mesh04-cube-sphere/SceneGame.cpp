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
    AssetsManager::LoadShader("standardMeshShader", "standardMeshShader.vert", "standardMeshShader.frag");
    cube.SetMaterialShader(0, AssetsManager::GetShader("standardMeshShader"));
    // Setting the roundness of the cube to half its size makes it a sphere
    cube.SetRoundness(5.0);
}

void SceneGame::Update(f32 dt) {
    camera.Update(dt);
}

void SceneGame::Draw() {
    gdraw::BeginMode3D(camera);

    gdraw::BeginShaderMode("standardMeshShader");
    cube.Draw();
    gdraw::EndShaderMode();

    gdraw::EndMode3D();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}