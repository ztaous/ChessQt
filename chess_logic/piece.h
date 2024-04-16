#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "colour.h"
#include "position.h"


class Board;


class Piece
{
public:
    Piece(Colour inputColour) : colour(inputColour) {};
    virtual ~Piece() = default;

    virtual std::vector<Position> getValidMoves(const Board& board) const = 0;
    
    void setPosition(const Position& newPosition, const Board& board) {};
    Position getPosition() const { return position; }
    
    Colour getColour() const { return colour; }
    bool isBlack() const { return colour == Colour::Black; }
    bool isWhite() const { return colour == Colour::White; }

protected:
    Colour colour;
    Position position;
};