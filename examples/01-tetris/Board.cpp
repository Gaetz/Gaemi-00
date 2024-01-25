//
// Created by Gaëtan Blaise-Cazalet on 24/01/2024.
//

#include "Board.hpp"

Board::Board(i32 rows, i32 cols) {
    data.reserve(rows);
    for (auto& v : data) {
        v.reserve(cols);
        for (i32 i = 0; i < cols; ++i) {
            v.push_back(0);
        }
    }
}
