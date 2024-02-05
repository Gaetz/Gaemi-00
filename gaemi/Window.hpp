//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_WINDOW_HPP
#define GAEMI_WINDOW_HPP

#include <string>
#include "Defines.hpp"

/**
 * Represent the game window.
 * This class is a wrapper around the raylib window.
 */
class Window {
public:
    /**
     * Create a new window and set it up
     * @param width The width of the window
     * @param height The height of the window
     * @param title The title of the window
     */
    Window(i32 width, i32 height, str title);

    /**
     * Destroy the window
     */
    ~Window();

    /**
     * When you want to know if the game window should close
     * @return True if the window should close, false otherwise
     */
    static bool ShouldClose();

private:
    i32 width;
    i32 height;
    str title;
};


#endif //GAEMI_WINDOW_HPP
