//
// Created by gaetz on 05/11/2022.
//

#include "Engine.hpp"
#include "Game.hpp"
#include "Log.hpp"

int main() {
    Log::Restart();

    Engine engine;
    engine.Start(1280, 720, "Procedural rounded cube", "assets/mesh03-rounded-cube", std::make_unique<Game>());

    return 0;
}
