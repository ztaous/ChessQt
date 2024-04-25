#include "knight.h"


namespace chess {

std::vector<Position> Knight::getValidMoves(const Board& board) const
{
    std::vector<Position> possibleMoves;
    Position currentPos = this->getPosition();

    std::vector<std::pair<int, int>> directions = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

    for (auto& dir: directions) {
        Position newPos = currentPos;

        newPos.x += dir.first;
        newPos.y += dir.second;

        if (!board.isPositionValid(newPos))
            continue;

        if (board.isOccupied(newPos)) {
            Piece* piece = board.getPiece(newPos);
            
            if (board.isOppositePiece(piece, this))
                possibleMoves.push_back(newPos);
            
            continue;
        }

        possibleMoves.push_back(newPos);
    }

    return possibleMoves;
}


}
