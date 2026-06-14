#pragma once

#include "board.h"
#include "piece.h"
#include <iostream>
#include <string>
#include <vector>

namespace chess
{

class Knight : public Piece
{
  public:
    using Piece::Piece;
    ~Knight() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override
    {
        return chess::PieceType::Knight;
    };
    std::string typeAsString() const override
    {
        return "Knight";
    }
};

} // namespace chess