#include "rook.h"
#include "board.h"


namespace chess {

std::vector<Position> Rook::getValidMoves(const Board& board) const
{
    std::vector<Position> possibleMoves;
    Position currentPos = getPosition();

    std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    
    
    for (auto& dir : directions) {
        Position newPos = currentPos;

        while (true) {
            newPos.x += dir.first;
            newPos.y += dir.second;

            if (!board.isPositionValid(newPos))
                break;

            if (board.isOccupied(newPos)) {
                if (board.isOppositePiece(newPos, this->getColour())) {
                    possibleMoves.push_back(newPos);
                }
                break;
            }

            possibleMoves.push_back(newPos);
        }
    }

    return possibleMoves;
}

}