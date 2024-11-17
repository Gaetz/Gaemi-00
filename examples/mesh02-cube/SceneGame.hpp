//
// Created by gaetz on 07/02/2024.
//

#ifndef MESH02_SCENE_GAME_HPP
#define MESH02_SCENE_GAME_HPP

#include <Cube3D.hpp>

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
    gdraw::Cube3D cube { 10, 10, 10, Vec3::zero };
};


#endif //MESH02_SCENE_GAME_HPP
