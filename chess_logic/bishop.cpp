#include "bishop.h"


namespace chess {

std::vector<Position> Bishop::getValidMoves(const Board& board) const
{
    std::vector<Position> possibleMoves;
    Position currentPos = this->getPosition();

    std::vector<std::pair<int, int>> directions = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
    
    
    for (auto& dir : directions) {
        Position newPos = currentPos;

        while (true) {
            newPos.x += dir.first;
            newPos.y += dir.second;

            if (!board.isPositionValid(newPos))
                break;

            if (board.isOccupied(newPos)) {
                Piece* piece = board.getPiece(newPos);
                
                if (board.isOppositePiece(piece, this)) {
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