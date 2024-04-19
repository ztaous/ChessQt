#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>

#include "piece.h"
#include "board.h"
#include "RAII_class.h"


namespace chess {

class King : public Piece
{
public:
    King(Colour inputColour, const Position& startPosition);
    ~King();
    
    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::King; }
    
    bool isPositionCheck(Position kingPosition, const Board& board) const;
    bool isCheck(const Board& board) const;

private:
    static int blackKingCount;
    static int whiteKingCount;
};

}