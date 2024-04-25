#include "king.h"


namespace chess {

int King::blackKingCount = 0;
int King::whiteKingCount = 0;

King::King(Colour inputColour, const Position& startPosition) : Piece(inputColour, startPosition)
{
    bool whiteKingMaxed = (inputColour == Colour::White && whiteKingCount >= 1);
    bool blackKingMaxed = (inputColour == Colour::Black && blackKingCount >= 1);
 
    if (whiteKingMaxed || blackKingMaxed)
        throw std::runtime_error("Instancier deux rois de la même couleur n'est pas permis");

    if (colour == Colour::White)
        whiteKingCount++;

    else if (colour == Colour::Black)
        blackKingCount++;
}


King::~King()
{
    if (colour == Colour::White)
        whiteKingCount--;

    else if (colour == Colour::Black)
        blackKingCount--;
}


std::vector<Position> King::getValidMoves(const Board& board) const
{
    std::vector<Position> possibleMoves;
    Position currentPos = this->getPosition();

    std::vector<std::pair<int, int>> directions = {{1,0}, {0,1}, {0, -1}, {-1, 0}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}};


    for (auto& dir : directions) {
        Position newPos = {currentPos.x + dir.first, currentPos.y + dir.second};

        if (board.isPositionValid(newPos) && (!board.isOccupied(newPos) || board.isOppositePiece(this, board.getPiece(newPos)))) {
            
            if (!isPositionCheck(newPos, board))
                possibleMoves.push_back(newPos);
        }
    }
         
    return possibleMoves;
}


bool King::isPositionCheck(Position kingPosition, const Board& board) const
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


bool King::isCheck(const Board& board) const
{
    Position currentPos = this->getPosition();
    return isPositionCheck(currentPos, board);
}

}