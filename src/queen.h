#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "piece.h"
#include "board.h"


namespace chess {

class Queen : public Piece
{
public:
    Queen(Colour inputColour, const Position& startPosition ) : Piece(inputColour, startPosition) {}
    ~Queen() = default;

    std::vector<Position> getValidMoves(const Board& board) const override;
    PieceType type() const override { return chess::PieceType::Queen; }
    std::string typeAsString() const override { return "Queen"; }
};

}

