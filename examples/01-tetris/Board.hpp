//
// Created by Gaëtan Blaise-Cazalet on 24/01/2024.
//

#ifndef TETRIS_BOARD_HPP
#define TETRIS_BOARD_HPP

#include "Defines.hpp"
#include "../../ghecs/Types.hpp"


class Board {
public:
    Board() = default;
    Board(i32 x, i32 y, i32 cols, i32 rows, f32 tileSize);

    void Reset();
    void Set(f32 xPos, f32 yPos, i32 color);

    bool IsOccupied(f32 xPos, f32 yPos);

private:
    i32 x;
    i32 y;
    f32 tileSize;
    vector<vector<i32>> data;
};


#endif // TETRIS_BOARD_HPP

