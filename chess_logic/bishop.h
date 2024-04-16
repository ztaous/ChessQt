#pragma once

#include <iostream>
#include <vector>
#include "piece.h"


class Bishop : public Piece
{
public:
    Bishop(Colour colour) : Piece(colour) {};
    ~Bishop() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
};
