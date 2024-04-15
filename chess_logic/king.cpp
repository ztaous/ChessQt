#include "king.h"


std::vector<Position> King::getValidMoves(const Board& board) const
{
    std::vector<Position> possibleMoves;
    Position currentPos = getPosition();

    std::vector<std::pair<int, int>> directions = {{1,0}, {0,1}, {0, -1}, {-1, 0}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}};


    for (auto& dir : directions) {
        Position newPos = {currentPos.x + dir.first, currentPos.y + dir.second};

        if (board.isPositionValid(newPos) && !board.isOccupied(newPos) || board.isOppositePiece(newPos, this->isWhite())) {
            if (/*safe move*/) {
                possibleMoves.push_back(newPos);
            }
        }
    }
         
    return possibleMoves;
}