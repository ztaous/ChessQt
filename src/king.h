#pragma once

#include <iostream>
#include <vector>
#include "piece.h"
#include "board.h"


namespace chess {

class King : public Piece
{
public:
    King(Colour inputColour, const Position& startPosition) : Piece(inputColour, startPosition) {}
    ~King() = default;
    
    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::King; }
    
    bool isPositionAttacked(Position kingPosition, const Board& board) const;
    bool otherKingAttack(Position kingPosition, const Board& board) const;
    
    bool isCheck(const Board& board) const;
    bool isCheckMate(const Board& board) const;
};

}