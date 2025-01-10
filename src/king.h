#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "piece.h"
#include "board.h"


namespace chess {

class King : public Piece
{
public:
    using Piece::Piece;
    ~King() = default;
    
    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::King; }
    std::string typeAsString() const override { return "King"; }
    
    bool isPositionAttacked(Position kingPosition, const Board& board) const;
    bool otherKingAttack(Position kingPosition, const Board& board) const;
    
    bool isCheck(const Board& board) const;
    bool isCheckMate(const Board& board) const;
};

}