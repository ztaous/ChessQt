#pragma once

#include <iostream>
#include <vector>
#include "piece.h"
#include "board.h"


namespace chess {

class tempMovePiece
{
public:
    tempMovePiece(Piece* tempPiece, const Position& newPosition) : piece(tempPiece), originalPosition(tempPiece->getPosition()) 
    {
        tempPiece->setPosition(newPosition);
    }

    ~tempMovePiece() 
    {
        piece->setPosition(originalPosition);
    }

private:
    Piece* piece;
    Position originalPosition;
};

}
