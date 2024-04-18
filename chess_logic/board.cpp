#include "board.h"

namespace chess {

Board::Board() : grid(rows, std::vector<Piece*>(columns, nullptr))
{
    grid[0][7] = new King(Colour::White);
    grid[4][3] = new Rook(Colour::Black);
    grid[2][1] = new Bishop(Colour::White);
}


Board::~Board()
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            delete grid[i][j];
        }
    }
}


bool Board::isOppositePiece(Position pos, Colour currentPieceColour) const
{
    const Piece* pieceAtPosition = getPiece(pos);
    
    if (pieceAtPosition == nullptr)
        return false;
    
    return pieceAtPosition->getColour() != currentPieceColour;
}


bool Board::movePiece(Position current, Position destination)
{
    // validation du mouvement
    if (!isPositionValid(current) || !isPositionValid(destination))
        return false;

    Piece* piece = getPiece(current);
    if (piece == nullptr)
        return false;
        
    std::vector<Position> validMoves = piece->getValidMoves(*this);
    if(std::find(validMoves.begin(), validMoves.end(), destination) == validMoves.end())
        return false;

    // capture des pieces
    Piece* targetPiece = getPiece(destination);
    if (targetPiece != nullptr) {
        if (piece->getColour() != targetPiece->getColour()) {
            delete targetPiece;
            grid[destination.y][destination.x] = nullptr;
        }
        else {
            return false;
        }
    }

    // bouge la piece si toutes les conditions sont respectees
    grid[destination.y][destination.x] = piece;
    grid[current.y][current.x] = nullptr;
    piece->setPosition(destination, *this);
    
    return true;
}

}
