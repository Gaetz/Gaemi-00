//
// Created by gaetz on 07/02/2024.
//

#ifndef SHADER01_SCENE_GAME_HPP
#define SHADER01_SCENE_GAME_HPP

#include "IScene.hpp"
#include "raylib.h"
#include "Game.hpp"
#include "../gecs/Types.hpp"
#include "Vec2.hpp"
#include "World.hpp"
#include "Cam3D.hpp"

using gmath::Vec2;
using gmath::Vec3;
using gecs::World;
using render::Cam3D;

class SceneGame : public IScene {
public:
    explicit SceneGame(Game& game);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    World& world { World::Instance() };
    Game& game;

    Model model {};
    Cam3D camera { Vec3 { -3.0f, 0.4f, -1.0f } };
    TextureCubemap skyboxTexture {};
    Model skyboxCube {};

    float totalTime { 0.0f };
};


#endif //SHADER01_SCENE_GAME_HPP
