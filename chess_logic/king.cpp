#include "king.h"
#include "board.h"


std::vector<Position> King::getValidMoves(const Board& board) const
{
    std::vector<Position> possibleMoves;
    Position currentPos = getPosition();

    std::vector<std::pair<int, int>> directions = {{1,0}, {0,1}, {0, -1}, {-1, 0}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}};


    for (auto& dir : directions) {
        Position newPos = {currentPos.x + dir.first, currentPos.y + dir.second};

        if (board.isPositionValid(newPos) && !board.isOccupied(newPos) || board.isOppositePiece(newPos, this->isWhite())) {
            // if (!isPositionCheck(newPos, board)) {
            //     possibleMoves.push_back(newPos);
            // }
            possibleMoves.push_back(newPos);
        }
    }
         
    return possibleMoves;
}


bool King::isPositionCheck(Position position, const Board& board) const
{
    // iterer sur toutes les pieces de la grille et verifier
    // qu'elles n'attaquent pas la nouvelle position
}


bool King::isCheck(const Board& board) const
{
    Position currentPos = this->getPosition();

    return isPositionCheck(currentPos, board);
}