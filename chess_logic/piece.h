#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include "colour.h"
#include "board.h"


struct Position
{
    int x = 0;
    int y = 0;
};


class Board;


class Piece
{
public:
    Piece(Colour inputColour) : colour(inputColour) {};
    virtual ~Piece() = default;

    virtual std::vector<Position> getValidMoves(const Board& board) const = 0;
    
    void setPosition(Position inputPosition, const Board& board) {};
    
    Position getPosition() const { return position; }
    Colour getColour() const { return colour; }
   
    bool isWhite() const { return colour == Colour::White; }
    bool isBlack() const { return colour == Colour::Black; }

protected:
    Colour colour;
    Position position;
};