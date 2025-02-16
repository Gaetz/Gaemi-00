//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.hpp"
#include "../ghecs/Entity.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "Query.hpp"
#include "GMath.hpp"

using ghecs::Position;
using ghecs::Velocity;
using ghecs::Sprite;
using ghecs::Query;
using gassets::AssetsManager;


SceneGame::SceneGame(Game& game) : game { game },
                                   ATOM_WIDTH { AssetsManager::GetData("ATOM_WIDTH") },
                                   SCREEN_WIDTH { static_cast<i32>(AssetsManager::GetData("SCREEN_WIDTH")) },
                                   SCREEN_HEIGHT { static_cast<i32>(AssetsManager::GetData("SCREEN_HEIGHT")) }
{
    world.Init();
    i32 atomSize { static_cast<i32>(ATOM_WIDTH) };;
    i32 maxCols = SCREEN_WIDTH / atomSize;
    i32 maxRows = SCREEN_HEIGHT / atomSize;
    grid.reserve(maxCols);
    for (i32 i = 0; i < maxCols; ++i) {
        grid.emplace_back(maxRows, 0);
    }
}

void SceneGame::Load() {
    i32 atomSize { static_cast<i32>(ATOM_WIDTH) };;
    Image imBlank = GenImageColor(atomSize, atomSize, WHITE);
    sandTexture = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);

}

void SceneGame::CreateSandAtom(i32 mousePixelX, i32 mousePixelY) {
    auto atomId = world.CreateEntity();
    i32 atomSize = static_cast<i32>(ATOM_WIDTH);
    i32 x = mousePixelX & ~(atomSize - 1);
    i32 y = mousePixelY & ~(atomSize - 1);
    x = gmath::Clamp<i32>(x, 0, SCREEN_WIDTH - atomSize);
    y = gmath::Clamp<i32>(y, 0, SCREEN_HEIGHT - atomSize);
    Position pos {static_cast<f32>(x), static_cast<f32>(y)};
    world.AddComponent<Position>(atomId, pos);
    Velocity vel {0, static_cast<f32>(ATOM_WIDTH)};
    world.AddComponent<Velocity>(atomId, vel);
    ghecs::Sprite sprite { sandTexture };
    world.AddComponent<Sprite>(atomId, sprite);
}

void SceneGame::Update(f32 dt) {

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        CreateSandAtom(GetMouseX(), GetMouseY());
    }

    auto q = Query<Position, Velocity>();
    q.RemoveIf<Velocity>([this](const Position& pos, const Velocity& vel) {
        i32 atomSize = static_cast<i32>(ATOM_WIDTH);
        const i32 x = static_cast<i32>(pos.x / ATOM_WIDTH);
        const i32 y = static_cast<i32>(pos.y / ATOM_WIDTH);

        if (y >= SCREEN_HEIGHT / atomSize - 1) {
            return true;
        }

        i32 below = grid[x][y+1];
        i32 belowRight = x+1 >= SCREEN_WIDTH / atomSize ? 1 : grid[x+1][y+1];
        i32 belowLeft = x-1 < 0 ? 1 : grid[x-1][y+1];
        return below == 1 && belowRight == 1 && belowLeft == 1;
    });

    q.Update([this](Position& pos, Velocity& vel) {
        const f32 unit = ATOM_WIDTH;
        i32 atomSize = static_cast<i32>(unit);
        const i32 x = static_cast<i32>(pos.x / ATOM_WIDTH);
        const i32 y = static_cast<i32>(pos.y / ATOM_WIDTH);
        vel.x = 0;
        vel.y = 0;

        i32 below = grid[x][y+1];

        if (below == 0) {
            vel.y = unit;
        } else {
            i32 dir = GetRandomValue(0, 1) * 2 - 1;
            if (x+dir < 0 || x+dir >= SCREEN_WIDTH / atomSize
             || x-dir < 0 || x-dir >= SCREEN_WIDTH / atomSize) {
                dir = 0;
            }

            i32 belowDir0 = grid[x+dir][y+1];
            i32 belowDir1 = grid[x-dir][y+1];
            if (belowDir0 == 0) {
                vel.x = unit * dir;
                vel.y = unit;
            } else if (belowDir1 == 0) {
                vel.x = -unit * dir;
                vel.y = unit;
            }
        }

        const Vec2 newPos { pos.x + vel.x, pos.y + vel.y };
        pos.Set(newPos);
    });
    q.Apply();

    ComputeGrid();

}

void SceneGame::ResetGrid() {
    for (auto& innerVec : grid) {
        std::fill(innerVec.begin(), innerVec.end(), 0);
    }
}

void SceneGame::ComputeGrid() {
    ResetGrid();
    auto q = Query<Position, Sprite>();
    q.Read([this](const Position& pos, const Sprite& sprite) {
        i32 atomSize = static_cast<i32>(ATOM_WIDTH);
        const i32 x = static_cast<i32>(pos.x / ATOM_WIDTH);
        const i32 y = static_cast<i32>(pos.y / ATOM_WIDTH);
        if (x < 0 || x >= SCREEN_WIDTH / atomSize || y < 0 || y >= SCREEN_HEIGHT / atomSize) {
            return;
        }
        grid[x][y] = 1;
    });
}

void SceneGame::Draw() {
    // Query all entities with a position and a sprite
    auto posSprites = ghecs::Query<Position, Sprite>();
    posSprites.Read([](const Position& pos, const Sprite& spr) {
        gdraw::DrawTexture(spr.texture, pos.x, pos.y, WHITE);
    });
}


void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}




