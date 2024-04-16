#pragma once

#include <iostream>
#include <vector>
#include "piece.h"


class King : public Piece
{
public:
    King(Colour colour) : Piece(colour) {}
    ~King() = default;
    
    std::vector<Position> getValidMoves(const Board& board) const override;
    
    bool isPositionCheck(Position position, const Board& board) const;
    bool isCheck(const Board& board) const;
};