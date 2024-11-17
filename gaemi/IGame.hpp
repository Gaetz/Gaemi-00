//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_I_GAME_HPP
#define GAEMI_I_GAME_HPP

#include "Defines.hpp"

/**
 * Game representation in engine. The game in implemented in a class that inherits from this interface.
 * This Game class should be coded in the game project, and instantiated in the main function.
 */
class IGame {
public:
    virtual ~IGame() = default;

    /**
     * Load the game resources
     */
    virtual void Load() = 0;

    /**
     * Update the game state
     * @param dt The time since the last frame
     */
    virtual void Update(f32 dt) = 0;

    /**
     * Call the game rendering
     */
    virtual void Draw() = 0;

    /**
     * Unload the game resources
     */
    virtual void Unload() = 0;
};

#endif //GAEMI_I_GAME_HPP
