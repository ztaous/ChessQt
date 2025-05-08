#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "piece.h"
#include "board.h"

namespace chess {

class Pawn : public Piece
{
public:
    using Piece::Piece;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::Pawn; }
    std::string typeAsString() const override { return "Pawn"; }

};

}