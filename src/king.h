#pragma once

#include "board.h"
#include "piece.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace chess
{

class King : public Piece
{
  public:
    using Piece::Piece;
    ~King() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override
    {
        return chess::PieceType::King;
    }
    std::string typeAsString() const override
    {
        return "King";
    }

    bool isPositionAttacked(Position kingPosition, const Board& board) const;
    bool otherKingAttack(Position kingPosition, const Board& board) const;

    bool isCheck(const Board& board) const;
    bool isCheckmate(const Board& board) const;
};

} // namespace chess