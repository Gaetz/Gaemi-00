//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENE_GAME_HPP
#define SUNSET_REVERIES_SCENE_GAME_HPP

#include "IScene.hpp"
#include "raylib.h"
#include "Game.hpp"
#include "../gecs/Types.hpp"
#include "Vec2.hpp"
#include "World.hpp"
#include "Board.hpp"

using gmath::Vec2;
using gecs::World;

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

    const i32 M;
    const i32 N;
    Board board;
};


#endif //SUNSET_REVERIES_SCENE_GAME_HPP
