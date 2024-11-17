//
// Created by gaetz on 07/02/2024.
//

#ifndef MESH03_SCENE_GAME_HPP
#define MESH03_SCENE_GAME_HPP

#include <RoundedCube3D.hpp>

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
    gdraw::RoundedCube3D cube { 10, 10, 10, 2.0, Vec3::zero };
};


#endif //MESH03_SCENE_GAME_HPP
