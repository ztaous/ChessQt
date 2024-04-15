#include "piece.h"


void Piece::setPosition(Position inputPosition, const Board& board)
{ 
    if (board.isPositionValid(inputPosition))
        position = inputPosition;
}