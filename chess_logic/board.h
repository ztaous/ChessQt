#pragma once

#include <iostream>
#include <vector>
#include "piece.h"


class Board
{
public:
    Board();
    ~Board();
    
    bool isOccupied(Position pos) const { return grid[pos.y][pos.x] != nullptr; }
    bool isOppositePiece(Position pos, bool currentPieceIsWhite) const;
    bool isPositionValid(Position pos) const { return (pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8); }

    Piece* getPiece(const Position& pos) const { return grid[pos.y][pos.x]; }

    int getRows() const { return rows; } 
    int getColumns() const { return columns; } 

private:
    std::vector<std::vector<Piece*>> grid;

    const int rows = 8;
    const int columns = 8;
};
