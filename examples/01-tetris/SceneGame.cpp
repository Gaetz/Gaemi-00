//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.hpp"
#include <iomanip>
#include "ImRenderer.h"
#include "../ghecs/World.hpp"
#include "../ghecs/Entity.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "Query.hpp"

using ghecs::Position;
using ghecs::Velocity;
using ghecs::Sprite;
using ghecs::Query;
using gassets::AssetsManager;

i32 figures[7][4]{
        1, 3, 5, 7,
        2, 4, 5, 7,
        3, 5, 4, 6,
        3, 5, 4, 7,
        2, 3, 5, 7,
        3, 5, 7, 6,
        2, 3, 4, 5
};

SceneGame::SceneGame(Game &game) : game{game},
                                   BOARD_WIDTH{static_cast<i32>(AssetsManager::GetData("BOARD_WIDTH"))},
                                   BOARD_HEIGHT{static_cast<i32>(AssetsManager::GetData("BOARD_HEIGHT"))},
                                   BOARD_X{static_cast<i32>(AssetsManager::GetData("BOARD_X"))},
                                   BOARD_Y{static_cast<i32>(AssetsManager::GetData("BOARD_Y"))},
                                   BOARD_TILE_SIZE{AssetsManager::GetData("BOARD_TILE_SIZE")} {


}

void SceneGame::Load() {
    AssetsManager::LoadTexture("background", "background.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("frame", "frame.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("tiles", "tiles.png", ToSceneId(SceneName::SceneGame));

    backgroundTexture = AssetsManager::GetTexture("background");
    board = Board(BOARD_X, BOARD_Y, BOARD_WIDTH, BOARD_HEIGHT, BOARD_TILE_SIZE);

    auto tileId = world.CreateEntity();
    Position pos{static_cast<f32>(BOARD_X), static_cast<f32>(BOARD_Y)};
    world.AddComponent<Position>(tileId, pos);
    ghecs::Sprite sprite{AssetsManager::GetTexture("tiles")};
    sprite.srcRect = Rect{0, 0, BOARD_TILE_SIZE, BOARD_TILE_SIZE};
    sprite.dstSize = Vec2{BOARD_TILE_SIZE, BOARD_TILE_SIZE};
    world.AddComponent<Sprite>(tileId, sprite);
    ghecs::Velocity vel{0, BOARD_TILE_SIZE};
    world.AddComponent<Velocity>(tileId, vel);
}

void SceneGame::Update(f32 dt) {
    timer += dt;
    if (timer > timeLimit) {
        auto &falling = world.Find<Position, Velocity, Sprite>();

        test++;
        if (test > 0 && test % 3 == 0) {
            auto tileId = world.CreateEntity();
            Position pos{static_cast<f32>(BOARD_X), static_cast<f32>(BOARD_Y)};
            world.AddComponent<Position>(tileId, pos);
            ghecs::Sprite sprite{AssetsManager::GetTexture("tiles")};
            sprite.srcRect = Rect{0, 0, BOARD_TILE_SIZE, BOARD_TILE_SIZE};
            sprite.dstSize = Vec2{BOARD_TILE_SIZE, BOARD_TILE_SIZE};
            world.AddComponent<Sprite>(tileId, sprite);
            ghecs::Velocity vel{0, BOARD_TILE_SIZE};
            world.AddComponent<Velocity>(tileId, vel);
            falling.Reset();
        }

        timer -= timeLimit;
        board.Reset();

        falling.Update([this](Position &pos, Velocity &vel, Sprite &spr) {
            pos.y += vel.y;
            board.Set(pos.x, pos.y, 1);
        });

        falling.RemoveIf<Velocity>([this](const Position &pos, const Velocity &vel, const Sprite &spr) {
            bool collides =  pos.y >= BOARD_Y + (BOARD_HEIGHT - 1) * BOARD_TILE_SIZE
                             || board.IsOccupied(pos.x, pos.y + vel.y);
            if(collides) {
                board.Set(pos.x, pos.y, -1);
            }
            return collides;
        });

        falling.Apply();
    }
}

void SceneGame::Draw() {
    gdraw::DrawTexture(backgroundTexture, 0, 0, WHITE);

    auto posSprites = world.Find<Position, Sprite>();
    posSprites.Reset();
    posSprites.Read([](const Position &pos, const Sprite &spr) {
        Rect dst{pos.x, pos.y, spr.dstSize.x, spr.dstSize.y};
        gdraw::DrawSprite(spr.texture, spr.srcRect, dst, WHITE);
    });
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}