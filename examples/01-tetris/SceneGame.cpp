//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.hpp"
#include <iomanip>
#include "ImRenderer.h"
#include "../gecs/World.hpp"
#include "../gecs/Entity.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "Query.hpp"

using gecs::Position;
using gecs::Velocity;
using gecs::Sprite;
using gecs::Query;

SceneGame::SceneGame(Game &game) : game{game},
                                   M{static_cast<i32>(AssetsManager::GetData("M"))},
                                   N{static_cast<i32>(AssetsManager::GetData("N"))},
                                   board{Board(M, N)} {

}

void SceneGame::Load() {
    AssetsManager::LoadTexture("background", "background.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("frame", "frame.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("tiles", "tiles.png", ToSceneId(SceneName::SceneGame));

    backgroundTexture = AssetsManager::GetTexture("background");

    vector<gecs::Id> entities;
    for (u32 i = 0; i < 50; ++i) {
        auto testEntityId = world.CreateEntity();
        gecs::Entity entity = world.GetEntity(testEntityId);
        Position pos{static_cast<f32>(GetRandomValue(0, 500)), static_cast<f32>(0 + i * 10)};
        world.AddComponent<gecs::Position>(testEntityId, pos);
        Velocity vel{static_cast<f32>(100), 0};
        world.AddComponent<gecs::Velocity>(testEntityId, vel);
        gecs::Sprite sprite{AssetsManager::GetTexture("tiles")};
        sprite.srcRect = Rect{0, 0, 18, 18};
        world.AddComponent<gecs::Sprite>(testEntityId, sprite);
        entities.push_back(testEntityId);
    }

    world.DestroyEntity(entities[20]);


}

void SceneGame::Update(f32 dt) {

    auto q = world.Find<Position, Velocity>();
    q.Update([dt](Position &pos, const Velocity &vel) {
        const Vec2 newPos{pos.x + vel.x * dt, pos.y + vel.y * dt};
        pos.Set(newPos);
    });
    q.Apply();

    q.DeleteIf([](const Position &pos, const Velocity &vel) {
        return pos.x > 700;
    });
}

void SceneGame::Draw() {
    render::DrawTexture(backgroundTexture, 0, 120, WHITE);

    auto posSprites = world.Find<Position, Sprite>();
    posSprites.Refresh();
    posSprites.Read([](const Position &pos, const Sprite &spr) {
        Rect dst{pos.x, pos.y, spr.srcRect.width, spr.srcRect.height};
        render::DrawSprite(spr.texture, spr.srcRect, dst, WHITE);
    });
}


void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}




