//
// Created by gaetz on 05/11/2022.
//

#include <algorithm>

#include "Engine.hpp"
#include "raylib.h"
#include "Window.hpp"
#include "IGame.hpp"
#include "Log.hpp"
#include "AssetsManager.hpp"
#include "Jobs.hpp"
#include "Renderer.hpp"

using gassets::AssetsManager;

void Engine::Start(i32 windowWidth, i32 windowHeight, const str& gameNameP,
                   const str& gameAssetsPathP, uptr<IGame>&& gameP) noexcept
{
    gameName = gameNameP;
    game = std::move(gameP);
    jobs::Initialize();

    Window window { windowWidth, windowHeight, gameName };
    SetTargetFPS(60);
    LOG(LogLevel::Info) << "Window initialized";

    AssetsManager::Initialize(gameAssetsPathP);
    AssetsManager::LoadData();
    game->Load();
    LOG(LogLevel::Info) << "Game loaded";

    while (!Window::ShouldClose())
    {
#ifdef GDEBUG
        f32 dt = std::min(GetFrameTime(), 0.0166f);
#else
        f32 dt = GetFrameTime();
#endif
        game->Update(dt);

        gdraw::BeginDraw();
        gdraw::ClearScreen();
        game->Draw();
        DrawFPS(50, 20);
        gdraw::EndDraw();
    }

    game->Unload();
    jobs::Close();

    LOG(LogLevel::Info) << "Bye :)";
}
