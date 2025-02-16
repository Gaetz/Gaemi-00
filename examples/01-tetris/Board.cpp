//
// Created by Gaëtan Blaise-Cazalet on 24/01/2024.
//

#include "Board.hpp"
#include "Entity.hpp"
#include "AssetsManager.hpp"

using ghecs::World;
using ghecs::Position;
using ghecs::Sprite;

Board::Board(i32 x, i32 y, i32 cols, i32 rows, f32 tileSize) :
        x{x}, y{y}, tileSize{tileSize} {
    data.reserve(rows);
    for (i32 i = 0; i < rows; ++i) {
        data.push_back(vector<i32>());
    }
    i32 row = 0;
    for (auto &v: data) {
        v.reserve(cols);
        for (i32 i = 0; i < cols; ++i) {
            v.push_back(0);
        }
        ++row;
    }
}

void Board::Reset() {
    for (auto &v: data) {
        for (auto &i: v) {
            if (i != -1) {
                i = 0;
            }
        }
    }
}

void Board::Set(f32 xPos, f32 yPos, i32 color) {
    const i32 col = (xPos - x) / tileSize;
    const i32 row = (yPos - y) / tileSize;
    data[row][col] = color;
}

bool Board::IsOccupied(f32 xPos, f32 yPos) {
    const i32 col = (xPos - x) / tileSize;
    const i32 row = (yPos - y) / tileSize;
    return data[row][col] != 0;
}

