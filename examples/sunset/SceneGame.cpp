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

SceneGame::SceneGame(Game& game) : game { game },
  PLAYER_JUMP_MAX_PRESS_TIME { AssetsManager::GetData("PLAYER_JUMP_MAX_PRESS_TIME") },
  PLAYER_HORIZONTAL_ACCELERATION { AssetsManager::GetData("PLAYER_HORIZONTAL_ACCELERATION") },
  PLAYER_JUMP_ACCELERATION { AssetsManager::GetData("PLAYER_JUMP_ACCELERATION") },
  PHYSICS_FRAME_REWIND_SPEED { static_cast<u32>(AssetsManager::GetData("PHYSICS_FRAME_REWIND_SPEED")) },
  PHYSICS_FRAME_FORWARD_SPEED { static_cast<u32>(AssetsManager::GetData("PHYSICS_FRAME_FORWARD_SPEED")) }
{

}

void SceneGame::Load() {
    AssetsManager::LoadTexture("bg_sunset", "assets/sunset/images/bg_sunset.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("player", "assets/sunset/images/player.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("ghost", "assets/sunset/images/ghost.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("projectile", "assets/sunset/images/projectile.png", ToSceneId(SceneName::SceneGame));

    backgroundTexture = AssetsManager::GetTexture("bg_sunset");

    ghecs::World& world = ghecs::World::Instance();
    world.Init();

    vector<ghecs::Id> entities;
    for (u32 i = 0; i < 50; ++i) {
        auto testEntityId = world.CreateEntity();
        ghecs::Entity entity = world.GetEntity(testEntityId);
        Position pos {static_cast<f32>(GetRandomValue(0, 500)), static_cast<f32>(0 + i * 10)};
        entity.AddComponent<ghecs::Position>(pos);
        Velocity vel {static_cast<f32>(100), 0};
        entity.AddComponent<ghecs::Velocity>(vel);
        ghecs::Sprite sprite { AssetsManager::GetTexture("ghost") };
        entity.AddComponent<ghecs::Sprite>(sprite);
        entities.push_back(testEntityId);
    }

    world.DestroyEntity(entities[20]);

    world.LogWorld();

}

void SceneGame::Update(f32 dt) {

    auto q = Query<Position, Velocity>();
    q.Each([dt](Position& pos, Velocity& vel) {
        const Vec2 newPos { pos.x + vel.x * dt, pos.y + vel.y * dt };
        pos.Set(newPos);
    });
    q.Apply();

    q.DeleteIf([](Position& pos, Velocity& vel) {
        return pos.x > 700;
    });
}

void SceneGame::Draw() {
    gdraw::DrawTexture(backgroundTexture, 0, 120, WHITE);

    // Query all entities with a position and a sprite
    auto posSprites = ghecs::Query<Position, Sprite>();
    posSprites.Each([](Position& pos, Sprite& spr) {
        gdraw::DrawTexture(spr.texture, pos.x, pos.y, WHITE);
    });
}


void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}




