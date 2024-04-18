#pragma once

#include <iostream>
#include <vector>
#include "piece.h"

#include "king.h"
#include "rook.h"
#include "bishop.h"


namespace chess {

class Board
{
public:
    Board();
    ~Board();
    
    bool isPositionValid(Position pos) const { return (pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8); }
    bool isOccupied(Position pos) const { return grid[pos.y][pos.x] != nullptr; }
    bool isOppositePiece(Position pos, Colour currentPieceColour) const;
   
    Piece* getPiece(const Position& pos) const { return grid[pos.y][pos.x]; }
    bool movePiece(Position current, Position destination);

    static const int rows = 8; 
    static const int columns = 8; 

private:
    std::vector<std::vector<Piece*>> grid;
};

}
