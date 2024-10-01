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
    engine.Start(1280, 720, "Noise Landscape", "assets/shader11-noise-landscape", std::make_unique<Game>());

    return 0;
}
