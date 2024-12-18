#include "board.h"


namespace chess {

Board::Board(QObject* parent) : QObject(parent), grid{}
{
    setupBoard(0);
}

Board::~Board()
{
    cleanBoard();
}

void Board::setupBoard(int scenario)
{
    cleanBoard();

    switch(scenario) {
        case 1:
            grid[1][5] = new King(Colour::Black, {5, 1});
            grid[7][2] = new King(Colour::White, {2, 7});
            grid[7][3] = new Rook(Colour::White, {3, 7});
            grid[6][3] = new Rook(Colour::White, {3, 6});
            grid[3][4] = new Bishop(Colour::Black, {4, 3});
            grid[2][6] = new Bishop(Colour::Black, {6, 2});
            break;
        
        case 2:
            grid[5][7] = new King(Colour::Black, {7, 5});
            grid[2][4] = new King(Colour::White, {4, 2});
            grid[2][1] = new Knight(Colour::Black, {1, 2});
            grid[1][5] = new Rook(Colour::White, {5, 1});
            break;

        case 3:
            grid[7][2] = new King(Colour::Black, {2, 7});
            grid[1][1] = new King(Colour::White, {1, 1});
            grid[5][0] = new Knight(Colour::White, {0, 5});
            grid[2][0] = new Rook(Colour::White, {0, 2});
            grid[4][3] = new Queen(Colour::Black, {3, 4});
            break;    
        
        default:
            grid[0][4] = new King(Colour::Black, {4, 0});
            grid[7][4] = new King(Colour::White, {3, 7});
            break;  
    }
}

void Board::cleanBoard()
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            delete grid[i][j];
            grid[i][j] = nullptr;
        }
    }
}

bool Board::isPositionValid(Position pos) const
{ 
    return (pos.x >= 0 && pos.x < columns && pos.y >= 0 && pos.y < rows); 
}

bool Board::isOccupied(Position pos) const 
{
    return grid[pos.y][pos.x] != nullptr;  
}

bool Board::isOppositePiece(const Piece* piece1, const Piece* piece2) const
{    
    return piece1->getColour() != piece2->getColour();
}

Piece* Board::getPiece(const Position& pos) const
{ 
    return grid[pos.y][pos.x]; 
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
    switchPlayer();
    
    return true;
}

void Board::switchPlayer()
{
    currentPlayer_ == Player::White ? currentPlayer_ = Player::Black : currentPlayer_ = Player::White; 
}

}
