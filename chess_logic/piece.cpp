#include "piece.h"
#include "board.h"


namespace chess {

void Piece::setPosition(const Position& newPosition, const Board& board)
{ 
    if (board.isPositionValid(newPosition))
        position = newPosition;
}

}
