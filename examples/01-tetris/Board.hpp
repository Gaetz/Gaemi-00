//
// Created by Gaëtan Blaise-Cazalet on 24/01/2024.
//

#ifndef TETRIS_BOARD_HPP
#define TETRIS_BOARD_HPP

#include "Defines.hpp"

class Board {
public:
    Board(i32 rows, i32 cols);

private:
    vector<vector<i32>> data;

    i32 figures[7][4] {
            1, 3, 5, 7,
            2, 4, 5, 7,
            3, 5, 4, 6,
            3, 5, 4, 7,
            2, 3, 5, 7,
            3, 5, 7, 6,
            2, 3, 4, 5
    };
};


#endif // TETRIS_BOARD_HPP

