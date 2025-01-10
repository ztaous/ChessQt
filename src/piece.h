#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>


namespace chess {

enum class Colour
{
    Black,
    White
};

enum class PieceType
{
    King,
    Queen,
    Bishop,
    Knight,
    Rook,
    Pawn
};

struct Position
{
    int x = 0;
    int y = 0;

    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }

    void operator +=(const Position& other)
    {
        x += other.x;
        y += other.y;
    }

    void operator -=(const Position& other)
    {
        x -= other.x;
        y -= other.y;
    }
};

class Board;

class Piece
{
public:
    Piece(Colour inputColour) : colour_(inputColour) {}
    Piece(Colour inputColour, const Position& startPosition) : colour_(inputColour), position_(startPosition) {}
    
    virtual ~Piece() = default;

    virtual std::vector<Position> getValidMoves(const Board& board) const = 0;
    virtual PieceType type() const = 0;
    virtual std::string typeAsString() const = 0;

    void setPosition(const Position& newPosition) { position_ = newPosition; }
    Position getPosition() const { return position_; }
    
    Colour getColour() const { return colour_; }
    bool isBlack() const { return colour_ == Colour::Black; }
    bool isWhite() const { return colour_ == Colour::White; }

protected:
    Colour colour_;
    Position position_;
};

}