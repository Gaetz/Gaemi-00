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

SceneGame::SceneGame(Game& game) : game { game },
  PLAYER_JUMP_MAX_PRESS_TIME { AssetsManager::GetData("PLAYER_JUMP_MAX_PRESS_TIME") },
  PLAYER_HORIZONTAL_ACCELERATION { AssetsManager::GetData("PLAYER_HORIZONTAL_ACCELERATION") },
  PLAYER_JUMP_ACCELERATION { AssetsManager::GetData("PLAYER_JUMP_ACCELERATION") },
  PHYSICS_FRAME_REWIND_SPEED { static_cast<u32>(AssetsManager::GetData("PHYSICS_FRAME_REWIND_SPEED")) },
  PHYSICS_FRAME_FORWARD_SPEED { static_cast<u32>(AssetsManager::GetData("PHYSICS_FRAME_FORWARD_SPEED")) }
{

}

void SceneGame::Load() {
    AssetsManager::LoadTexture("background", "background.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("frame", "frame.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("tiles", "tiles.png", ToSceneId(SceneName::SceneGame));

    backgroundTexture = AssetsManager::GetTexture("background");

    gecs::World& world = gecs::World::Instance();
    world.Init();

    vector<gecs::Id> entities;
    for (u32 i = 0; i < 50; ++i) {
        auto testEntityId = world.CreateEntity();
        gecs::Entity entity = world.GetEntity(testEntityId);
        Position pos {static_cast<f32>(GetRandomValue(0, 500)), static_cast<f32>(0 + i * 10)};
        entity.AddComponent<gecs::Position>(pos);
        Velocity vel {static_cast<f32>(100), 0};
        entity.AddComponent<gecs::Velocity>(vel);
        gecs::Sprite sprite { AssetsManager::GetTexture("tiles") };
        sprite.srcRect = Rect { 0, 0, 16, 16 };
        entity.AddComponent<gecs::Sprite>(sprite);
        entities.push_back(testEntityId);
    }

    world.DestroyEntity(entities[20]);

    world.LogWorld();

}

void SceneGame::Update(f32 dt) {

    auto q = Query<Position, Velocity>();
    q.Update([dt](Position& pos, const Velocity& vel) {
        const Vec2 newPos { pos.x + vel.x * dt, pos.y + vel.y * dt };
        pos.Set(newPos);
    });
    q.Apply();

    q.DeleteIf([](const Position& pos, const Velocity& vel) {
        return pos.x > 700;
    });
}

void SceneGame::Draw() {
    render::DrawTexture(backgroundTexture, 0, 120, WHITE);

    /// TODO World should store queries and update them when needed

    auto posSprites = Query<Position, Sprite>();
    posSprites.Refresh();
    posSprites.Read([](const Position& pos, const Sprite& spr) {
        Rect dst { pos.x, pos.y, spr.srcRect.width, spr.srcRect.height };
        render::DrawSprite(spr.texture, spr.srcRect, dst, WHITE);
    });
}


void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}




