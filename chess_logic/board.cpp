#include <board.h>


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


bool Board::isOppositePiece(Position pos, bool currentPieceIsWhite) const
{
    const  Piece* pieceAtPosition = getPiece(pos);
    
    if (pieceAtPosition == nullptr)
        return false;
    
    return pieceAtPosition->isWhite() != currentPieceIsWhite;
}

