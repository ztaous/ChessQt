#pragma once

#include <iostream>
#include <vector>

#include "piece.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"


namespace chess {

class Board
{
public:
    Board();
    ~Board();
    void cleanBoard();
    void setupBoard(int scenario);
    
    bool isPositionValid(Position pos) const;
    bool isOccupied(Position pos) const;
    bool isOppositePiece(const Piece* piece1, const Piece* piece2) const;
   
    Piece* getPiece(const Position& pos) const;
    bool movePiece(Position current, Position destination);

    static const int rows = 8; 
    static const int columns = 8; 

private:
    Piece* grid[rows][columns];
};

}
