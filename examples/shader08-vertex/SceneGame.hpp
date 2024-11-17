//
// Created by gaetz on 07/02/2024.
//

#ifndef SHADER01_SCENE_GAME_HPP
#define SHADER01_SCENE_GAME_HPP

#include <AssetsManager.hpp>

#include "IScene.hpp"
#include "raylib.h"
#include "Game.hpp"
#include "Vec2.hpp"
#include "Cam3D.hpp"
#include "Model3D.hpp"

using gmath::Vec2;
using gmath::Vec3;
using gdraw::Cam3D;
using gassets::Model3D;
using gassets::AssetsManager;

class SceneGame : public IScene
{
public:
    explicit SceneGame(Game& game);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    Game& game;

    Model3D model {};
    Cam3D camera { Vec3 { -3.0f, 1.0f, -1.0f }, Vec3 { 0.0f, -0.25f, 0.0f } };
    TextureCubemap skyboxTexture {};
    Model3D skyboxCube {};

    float totalTime { 0.0f };
};


#endif //SHADER01_SCENE_GAME_HPP
