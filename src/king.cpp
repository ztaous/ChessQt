#include "king.h"
#include "tempPieceMove.h"

namespace chess
{

std::vector<Position> King::getValidMoves(const Board& board) const
{
    std::vector<Position> possibleMoves;
    Position currentPos = this->getPosition();

    std::vector<std::pair<int, int>> directions = {{1, 0}, {0, 1},   {0, -1}, {-1, 0},
                                                   {1, 1}, {-1, -1}, {-1, 1}, {1, -1}};

    for (auto& dir : directions) {
        Position newPos = {currentPos.x + dir.first, currentPos.y + dir.second};

        if (board.isPositionValid(newPos) &&
            (!board.isOccupied(newPos) || board.isOppositePiece(this, board.getPiece(newPos)))) {
            TempPieceMove temp(const_cast<King*>(this), newPos);

            if (!isPositionAttacked(newPos, board) && !otherKingAttack(newPos, board))
                possibleMoves.push_back(newPos);
        }
    }

    return possibleMoves;
}

bool King::isPositionAttacked(Position kingPosition, const Board& board) const
{
    for (int i = 0; i < Board::rows; i++) {
        for (int j = 0; j < Board::columns; j++) {

            Piece* piece = board.getPiece({j, i});

            if (piece != nullptr && board.isOppositePiece(piece, this) && piece->type() != PieceType::King) {
                std::vector<Position> dangerMoves = piece->getValidMoves(board);

                if (std::find(dangerMoves.begin(), dangerMoves.end(), kingPosition) != dangerMoves.end())
                    return true;
            }
        }
    }

    return false;
}

bool King::otherKingAttack(Position kingPosition, const Board& board) const
{
    std::vector<std::pair<int, int>> directions = {{1, 0}, {0, 1},   {0, -1}, {-1, 0},
                                                   {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    for (auto& dir : directions) {
        Position checkPos = {kingPosition.x + dir.first, kingPosition.y + dir.second};
        if (board.isPositionValid(checkPos)) {
            Piece* piece = board.getPiece(checkPos);
            if (piece && piece->type() == PieceType::King && board.isOppositePiece(piece, this)) {
                return true;
            }
        }
    }
    return false;
}

bool King::isCheck(const Board& board) const
{
    Position currentPos = this->getPosition();
    return isPositionAttacked(currentPos, board) || otherKingAttack(currentPos, board);
}

bool King::isCheckmate(const Board& board) const
{
    if (!isCheck(board))
        return false;

    std::vector<Position> moves = this->getValidMoves(board);
    return moves.empty();
}
} // namespace chess