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
    engine.Start(1280, 720, "Shader trigonometrics", "assets/shader05-trigo", std::make_unique<Game>());

    return 0;
}
