#pragma once

#include "board.h"
#include "piece.h"
#include <iostream>
#include <string>
#include <vector>

namespace chess
{

class Queen : public Piece
{
  public:
    using Piece::Piece;
    ~Queen() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override
    {
        return chess::PieceType::Queen;
    }
    std::string typeAsString() const override
    {
        return "Queen";
    }
};

} // namespace chess
