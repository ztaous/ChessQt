#pragma once

#include <iostream>
#include <vector>
#include "piece.h"
#include "board.h"


class Rook : public Piece
{
public:
    Rook(Colour colour) : Piece(colour) {};
    ~Rook() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
};