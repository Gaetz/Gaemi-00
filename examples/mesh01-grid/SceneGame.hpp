//
// Created by gaetz on 07/02/2024.
//

#ifndef MESH01_SCENE_GAME_HPP
#define MESH01_SCENE_GAME_HPP

#include "IScene.hpp"
#include "raylib.h"
#include "Game.hpp"
#include "Vec2.hpp"
#include "Cam3D.hpp"
#include "Grid3D.hpp"

using gmath::Vec2;
using render::Cam3D;

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
    Grid3D grid { 10, 10, true };
    //Texture2D shaderTexture;
    float totalTime { 0.0f };
};


#endif //MESH01_SCENE_GAME_HPP
