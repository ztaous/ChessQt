#include "rook.h"

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
                if (board.isOppositePiece(newPos, this->isWhite())) {
                    possibleMoves.push_back(newPos);
                }
                break;
            }

            possibleMoves.push_back(newPos);
        }
    }

    return possibleMoves;
}