//
// Created by gaetz on 07/02/2024.
//

#ifndef SHADER01_SCENE_GAME_HPP
#define SHADER01_SCENE_GAME_HPP

#include "IScene.hpp"
#include "raylib.h"
#include "Game.hpp"
#include "../ghecs/Types.hpp"
#include "Vec2.hpp"
#include "World.hpp"

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
    Texture2D shaderTexture;
};


#endif //SHADER01_SCENE_GAME_HPP
