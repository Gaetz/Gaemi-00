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
    //DisableCursor();
    AssetsManager::LoadShader("deformableMeshShader", "deformableMeshShader.vert", "standardMeshShader.frag");
    sphere.SetMaterialShader(0, AssetsManager::GetShader("deformableMeshShader"));
}

void SceneGame::Update(f32 dt) {

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        camera.Update(dt);
    }

    collision.hit = false;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Ray raycast = GetMouseRay(GetMousePosition(), camera.ToRaylib());

        collision = GetRayCollisionSphere(raycast, sphere.GetPosition().ToRaylib(), sphere.GetRadius());
    }

}

void SceneGame::Draw() {
    gdraw::BeginMode3D(camera);

    if (collision.hit) {
        Vec3 endPoint = Vec3 { collision.point } + Vec3 { collision.normal } * 3.0f;
        DrawLine3D(collision.point, endPoint.ToRaylib(), RED);
        DrawSphere(collision.point, 0.1f, RED);
    }

    gdraw::BeginShaderMode("deformableMeshShader");
    if (collision.hit) {
        gdraw::SetShaderInt("deformableMeshShader", "hit", 1);
        gdraw::SetShaderVec3("deformableMeshShader", "hitPosition", Vec3 { collision.point });
        gdraw::SetShaderVec3("deformableMeshShader", "hitNormal", Vec3 { collision.normal });
    } else {
        gdraw::SetShaderInt("deformableMeshShader", "hit", 0);
        gdraw::SetShaderVec3("deformableMeshShader", "hitPosition", Vec3::zero);
        gdraw::SetShaderVec3("deformableMeshShader", "hitNormal", Vec3::zero);
    }
    sphere.Draw();
    gdraw::EndShaderMode();

    gdraw::EndMode3D();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}