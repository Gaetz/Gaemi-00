//
// Created by gaetz on 06/11/2022.
//

#ifndef SANDFALL_SCENE_GAME_HPP
#define SANDFALL_SCENE_GAME_HPP

#include "IScene.hpp"
#include "raylib.h"
#include "Game.hpp"
#include "../ghecs/Types.hpp"
#include "Vec2.hpp"
#include "World.hpp"

using gmath::Vec2;

enum class TimeStatus {
    Normal,
    Rewinding,
    Pause,
    Forward
};

using ghecs::World;

class SceneGame : public IScene {
public:
    explicit SceneGame(Game& game);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    Game& game;
    World& world { World::Instance() };

    const f32 ATOM_WIDTH;
    const i32 SCREEN_WIDTH;
    const i32 SCREEN_HEIGHT;

    vector<vector<i32>> grid {};
    vector<vector<i32>> nextGrid {};
    Texture sandTexture {};

    void CreateSandAtom(i32 mousePixelX, i32 mousePixelY);
    void ResetGrid();
    void ComputeGrid();
};


#endif //SANDFALL_SCENE_GAME_HPP
