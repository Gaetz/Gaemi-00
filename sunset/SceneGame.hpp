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

using gmath::Vec2;

enum class TimeStatus {
    Normal,
    Rewinding,
    Pause,
    Forward
};

class SceneGame : public IScene {
public:
    SceneGame(Game& game);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    gecs::Id testEntityId;

    Game& game;
    Texture2D backgroundTexture {};

    u32 currentFrame { 0 };
    u32 maxCurrentFrame { 0 };
    TimeStatus timeStatus { TimeStatus::Normal };

    const f32 PLAYER_JUMP_MAX_PRESS_TIME;
    const f32 PLAYER_HORIZONTAL_ACCELERATION;
    const f32 PLAYER_JUMP_ACCELERATION;

    const u32 PHYSICS_FRAME_REWIND_SPEED;
    const u32 PHYSICS_FRAME_FORWARD_SPEED;
};


#endif //SUNSET_REVERIES_SCENE_GAME_HPP
