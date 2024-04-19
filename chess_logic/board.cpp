#include "board.h"

namespace chess {

Board::Board() : grid(rows, std::vector<Piece*>(columns, nullptr))
{
    try {
        grid[0][7] = new King(Colour::White, {7, 0});
        grid[4][3] = new Rook(Colour::Black, {3, 4});
        grid[2][1] = new Bishop(Colour::White, {1, 2});
        
        // test d'exception
        // grid[2][7] = new King(Colour::White, {7, 2});
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        cleanBoard();
    }     
}


Board::~Board()
{
    cleanBoard();
}


void Board::cleanBoard()
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            delete grid[i][j];
            grid[i][j] = nullptr;
        }
    }
}


bool Board::isOppositePiece(const Piece* piece1, const Piece* piece2) const
{    
    return piece1->getColour() != piece2->getColour();
}


bool Board::movePiece(Position current, Position destination)
{
    
    if (!isPositionValid(current) || !isPositionValid(destination))
        return false;

    Piece* piece = getPiece(current);
    if (piece == nullptr)
        return false;
        
    std::vector<Position> validMoves = piece->getValidMoves(*this);
    if(std::find(validMoves.begin(), validMoves.end(), destination) == validMoves.end())
        return false;

    
    Piece* targetPiece = getPiece(destination);
    if (targetPiece != nullptr && isOppositePiece(targetPiece, piece)) {
        delete targetPiece;
        grid[destination.y][destination.x] = nullptr;
    }

    grid[destination.y][destination.x] = piece;
    grid[current.y][current.x] = nullptr;
    piece->setPosition(destination);
    
    return true;
}

}
