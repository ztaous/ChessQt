#pragma once

#include <iostream>
#include <vector>
#include "piece.h"


namespace chess {

class King : public Piece
{
public:
    explicit King(Colour colour) : Piece(colour) {}
    ~King() = default;
    
    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::King; }
    
    bool isPositionCheck(Position position, const Board& board) const;
    bool isCheck(const Board& board) const;
};

}