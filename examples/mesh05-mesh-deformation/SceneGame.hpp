//
// Created by gaetz on 07/02/2024.
//

#ifndef MESH04_SCENE_GAME_HPP
#define MESH04_SCENE_GAME_HPP

#include <SphereCube3D.hpp>

#include "IScene.hpp"
#include "Game.hpp"
#include "Vec2.hpp"
#include "Cam3D.hpp"

using gmath::Vec2;
using gdraw::Cam3D;

class SceneGame : public IScene {
public:
    explicit SceneGame(Game& game);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    Game& game;
    Cam3D camera {};
    gdraw::SphereCube3D sphere { 10, 10.0f, Vec3 { 0.0f, 0.0f, 20.0f } };
    RayCollision collision { false, 0.0f, Vec3::zero.ToRaylib(), Vec3::zero.ToRaylib() };
};


#endif //MESH03_SCENE_GAME_HPP
