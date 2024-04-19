#pragma once

#include <iostream>
#include <vector>
#include "piece.h"
#include "board.h"


namespace chess {

class Bishop : public Piece
{
public:
    Bishop(Colour inputColour, const Position& startPosition) : Piece(inputColour, startPosition) {};
    ~Bishop() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::Bishop; }
};

}
