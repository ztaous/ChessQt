#include "pawn.h"

namespace chess
{

std::vector<Position> Pawn::getValidMoves(const Board& board) const
{
    std::vector<Position> possibleMoves;
    Position currentPos = this->getPosition();

    int direction = (this->getColour() == Colour::White) ? -1 : 1; // direction dépend de la couleur (board flipped)
    bool isFirstMove = (this->getColour() == Colour::White && currentPos.y == 6) ||
                       (this->getColour() == Colour::Black &&
                        currentPos.y == 1); // vérifie si les pions sur la seconde ou avant dernière rangée

    // Avancer
    Position moveUpOne = {currentPos.x, currentPos.y + direction};
    if (board.isPositionValid(moveUpOne) && !board.isOccupied(moveUpOne)) {
        possibleMoves.push_back(moveUpOne);
    }

    if (isFirstMove) {
        Position moveUpTwo = {currentPos.x, currentPos.y + 2 * direction};
        if (board.isPositionValid(moveUpTwo) && !board.isOccupied(moveUpTwo)) {
            possibleMoves.push_back(moveUpTwo);
        }
    }

    // Capturer
    for (int diagonalDirection : {-1, 1}) {
        Position capturePos = {currentPos.x + diagonalDirection, currentPos.y + direction};
        if (board.isPositionValid(capturePos)) {
            const Piece* target = board.getPiece(capturePos);
            if (target && board.isOppositePiece(this, target)) {
                possibleMoves.push_back(capturePos);
            }
        }
    }

    return possibleMoves;
}

} // namespace chess
