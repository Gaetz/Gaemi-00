//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_ENGINE_HPP
#define GAEMI_ENGINE_HPP

#include <string>
#include "Defines.hpp"

class IGame;

class Engine {
public:
    Engine() = default;
    Engine(Engine&) = delete;
    Engine& operator=(Engine&) = delete;

    /**
     * Start the game engine and run the game loop
     * @param windowWidth The width of the window
     * @param windowHeight The height of the window
     * @param gameName The name of the game
     * @param gameAssetsPath The path to the game assets
     * @param gameP Here you move the game instance unique pointer
     */
    GAPI void Start(i32 windowWidth, i32 windowHeight, const str& gameName,
                    const str& gameAssetsPath, uptr<IGame>&& gameP) noexcept;

    /** Game name storage */
    str gameName;

    /** Game instance */
    uptr<IGame> game;
};


#endif //GAEMI_ENGINE_HPP
