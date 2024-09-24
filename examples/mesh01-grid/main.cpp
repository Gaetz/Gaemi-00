//
// Created by gaetz on 05/11/2022.
//

#include "Engine.hpp"
#include "Game.hpp"
#include "Log.hpp"

int main()
{
    Log::Restart();

    Engine engine;
    engine.Start(1280, 720, "Shader 2D Signed Distance Field", "assets/shader09-2d-sdf", std::make_unique<Game>());

    return 0;
}
