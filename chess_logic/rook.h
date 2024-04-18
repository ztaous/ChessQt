#pragma once

#include <iostream>
#include <vector>
#include "piece.h"


namespace chess {

class Rook : public Piece
{
public:
    explicit Rook(Colour colour) : Piece(colour) {};
    ~Rook() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::Rook; }
};

}