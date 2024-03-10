//
// Created by gaetz on 05/11/2022.
//

#include "../gaemi/Engine.hpp"
#include "Game.hpp"
#include "Log.hpp"

int main()
{
    Log::Restart();

    Engine engine;
    engine.Start(1280, 720, "Sandfall", "assets/sandfall", std::make_unique<Game>());

    return 0;
}