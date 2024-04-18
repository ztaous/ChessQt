#pragma once

#include <iostream>
#include <vector>
#include "piece.h"


namespace chess {

class Bishop : public Piece
{
public:
    explicit Bishop(Colour colour) : Piece(colour) {};
    ~Bishop() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::Bishop; }
};

}
