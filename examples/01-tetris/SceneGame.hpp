//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENE_GAME_HPP
#define SUNSET_REVERIES_SCENE_GAME_HPP

#include "IScene.hpp"
#include "raylib.h"
#include "Game.hpp"
#include "../ghecs/Types.hpp"
#include "Vec2.hpp"
#include "World.hpp"
#include "Board.hpp"

using gmath::Vec2;
using ghecs::World;

class SceneGame : public IScene {
public:
    SceneGame(Game& game);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    World& world { World::Instance() };
    Game& game;
    Texture2D backgroundTexture {};

    const i32 BOARD_X;
    const i32 BOARD_Y;
    const i32 BOARD_WIDTH;
    const i32 BOARD_HEIGHT;
    const f32 BOARD_TILE_SIZE;
    Board board;
    f32 timer { 0 };
    f32 timeLimit { 0.5 };
    i32 test { 0 };
};


#endif //SUNSET_REVERIES_SCENE_GAME_HPP
