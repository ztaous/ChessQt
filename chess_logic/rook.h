#pragma once

#include <iostream>
#include <vector>
#include "piece.h"
#include "board.h"


namespace chess {

class Rook : public Piece
{
public:
    Rook(Colour inputColour, const Position& startPosition) : Piece(inputColour, startPosition) {};
    ~Rook() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::Rook; }
};

}