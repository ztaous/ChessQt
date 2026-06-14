#pragma once

#include "board.h"
#include "piece.h"
#include <iostream>
#include <string>
#include <vector>

namespace chess
{

class Bishop : public Piece
{
  public:
    using Piece::Piece;
    ~Bishop() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override
    {
        return chess::PieceType::Bishop;
    }
    std::string typeAsString() const override
    {
        return "Bishop";
    }
};

} // namespace chess
