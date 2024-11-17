//
// Created by gaetz on 05/11/2022.
//

#include "Engine.hpp"
#include "Game.hpp"
#include "Log.hpp"

int main() {
    Log::Restart();

    Engine engine;
    engine.Start(1280, 720, "Procedural sphere", "assets/mesh04-cube-sphere", std::make_unique<Game>());

    return 0;
}
