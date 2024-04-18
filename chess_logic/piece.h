#pragma once

#include <iostream>
#include <vector>
#include <algorithm>


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

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};


class Board;

class Piece
{
public:
    explicit Piece(Colour inputColour) : colour(inputColour) {};
    virtual ~Piece() = default;

    virtual std::vector<Position> getValidMoves(const Board& board) const = 0;
    virtual PieceType type() const = 0;

    void setPosition(const Position& newPosition, const Board& board) {};
    Position getPosition() const { return position; }
    
    Colour getColour() const { return colour; }
    bool isBlack() const { return colour == Colour::Black; }
    bool isWhite() const { return colour == Colour::White; }

protected:
    Colour colour;
    Position position;
};

}