#pragma once

#include "board.h"
#include "piece.h"
#include <iostream>
#include <string>
#include <vector>

namespace chess
{

class Pawn : public Piece
{
  public:
    using Piece::Piece;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override
    {
        return chess::PieceType::Pawn;
    }
    std::string typeAsString() const override
    {
        return "Pawn";
    }
};

} // namespace chess