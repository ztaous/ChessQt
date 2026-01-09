#pragma once

#include "board.h"
#include "piece.h"
#include <iostream>
#include <string>
#include <vector>

namespace chess
{

class Rook : public Piece
{
  public:
    using Piece::Piece;
    ~Rook() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override
    {
        return chess::PieceType::Rook;
    }
    std::string typeAsString() const override
    {
        return "Rook";
    }
};

} // namespace chess
