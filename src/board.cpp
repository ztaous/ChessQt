#include "board.h"

namespace chess
{

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

    switch (scenario) {
        case 1:
            addPiece("f7", new King(Colour::Black));
            addPiece("c1", new King(Colour::White));
            addPiece("d1", new Rook(Colour::White));
            addPiece("d2", new Rook(Colour::White));
            addPiece("e5", new Bishop(Colour::Black));
            addPiece("g6", new Bishop(Colour::Black));
            break;

        case 2:
            addPiece("h3", new King(Colour::Black));
            addPiece("e6", new King(Colour::White));
            addPiece("b6", new Knight(Colour::Black));
            addPiece("f7", new Rook(Colour::White));
            break;

        case 3:
            addPiece("c1", new King(Colour::Black));
            addPiece("b8", new King(Colour::White));
            addPiece("a4", new Knight(Colour::White));
            addPiece("a6", new Rook(Colour::White));
            addPiece("d4", new Queen(Colour::Black));
            break;

        default:
            // White Pieces
            for (int i = 0; i < 8; i++) {
                std::string position = std::string(1, 'a' + i) + "2";
                addPiece(position, new Pawn(Colour::White));
            }
            addPiece("e1", new King(Colour::White));
            addPiece("d1", new Queen(Colour::White));
            addPiece("a1", new Rook(Colour::White));
            addPiece("h1", new Rook(Colour::White));
            addPiece("b1", new Knight(Colour::White));
            addPiece("g1", new Knight(Colour::White));
            addPiece("c1", new Bishop(Colour::White));
            addPiece("f1", new Bishop(Colour::White));

            // Black Pieces
            for (int i = 0; i < 8; i++) {
                std::string position = std::string(1, 'a' + i) + "7";
                addPiece(position, new Pawn(Colour::Black));
            }
            addPiece("e8", new King(Colour::Black));
            addPiece("d8", new Queen(Colour::Black));
            addPiece("a8", new Rook(Colour::Black));
            addPiece("h8", new Rook(Colour::Black));
            addPiece("b8", new Knight(Colour::Black));
            addPiece("g8", new Knight(Colour::Black));
            addPiece("c8", new Bishop(Colour::Black));
            addPiece("f8", new Bishop(Colour::Black));
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

void Board::addPiece(const std::string& notation, Piece* piece)
{
    Position pos = convertNotationToGrid(notation);
    if (!isPositionValid(pos)) {
        delete piece;
        return;
    }
    if (grid[pos.y][pos.x] != nullptr) {
        delete piece;
        return;
    }
    grid[pos.y][pos.x] = piece;
    piece->setPosition(pos);
}

Position Board::convertNotationToGrid(const std::string& notation)
{
    if (notation.length() != 2) {
        std::cerr << "Invalid notation format" << std::endl;
        return {-1, -1};
    }
    char colChar = notation[0];
    char rowChar = notation[1];

    Position pos;
    pos.x = colChar - 'a';
    pos.y = 8 - (rowChar - '0');
    return pos;
}

void Board::removePiece(const Position& pos)
{
    Piece* piece = getPiece(pos);
    delete piece;
    grid[pos.y][pos.x] = nullptr;
}

bool Board::movePiece(Position current, Position destination)
{
    if (!isPositionValid(current) || !isPositionValid(destination))
        return false;

    Piece* piece = getPiece(current);
    if (!piece)
        return false;

    if (!canMove(piece->getColour() == Colour::White ? Player::White : Player::Black))
        return false;

    std::vector<Position> validMoves = piece->getValidMoves(*this);
    if (std::find(validMoves.begin(), validMoves.end(), destination) == validMoves.end())
        return false;

    Piece* targetPiece = getPiece(destination);
    if (targetPiece && isOppositePiece(targetPiece, piece)) {
        if (targetPiece->type() == PieceType::King)
            return false;
    }

    if (targetPiece) {
        delete targetPiece;
        grid[destination.y][destination.x] = nullptr;
    }

    grid[destination.y][destination.x] = piece;
    grid[current.y][current.x] = nullptr;
    piece->setPosition(destination);

    switchPlayer();

    const Player currentPlayer = getCurrentPlayer();
    if (isCheckmate(currentPlayer)) {
        emit gameOver(opponentOf(currentPlayer));
    }

    return true;
}

void Board::switchPlayer()
{
    currentPlayer_ = (currentPlayer_ == Player::White) ? Player::Black : Player::White;
    emit playerChanged(currentPlayer_);
}

bool Board::canMove(Player player) const
{
    if (currentGameMode_ == GameMode::practice)
        return true;
    return player == currentPlayer_;
}

std::vector<Position> Board::legalMovesFrom(Position from)
{
    std::vector<Position> out;

    Piece* moving = getPiece(from);
    if (!moving)
        return out;

    std::vector<Position> candidates = moving->getValidMoves(*this);
    const Player mover = getCurrentPlayer();
    const Position saved = moving->getPosition();

    for (const Position& to : candidates) {
        Piece* captured = getPiece(to);

        grid[to.y][to.x] = moving;
        grid[from.y][from.x] = nullptr;
        moving->setPosition(to);

        const bool illegal = isInCheck(mover);

        grid[from.y][from.x] = moving;
        grid[to.y][to.x] = captured;
        moving->setPosition(saved);

        if (!illegal)
            out.push_back(to);
    }
    return out;
}

Position Board::findKing(Player player) const
{
    Colour colour = colourOf(player);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            Piece* piece = grid[row][col];
            if (piece && piece->type() == PieceType::King && piece->getColour() == colour) {
                return {col, row};
            }
        }
    }
    return {-1, -1};
}

bool Board::isInCheck(Player player) const
{
    Position kingPos = findKing(player);
    if (kingPos.x < 0 || kingPos.y < 0)
        return false;

    Piece* kingPiece = getPiece(kingPos);
    if (!kingPiece || kingPiece->type() != PieceType::King)
        return false;

    const King* king = static_cast<const King*>(kingPiece);
    return king->isPositionAttacked(kingPos, *this) || king->otherKingAttack(kingPos, *this);
}

bool Board::isCheckmate(Player player) const
{
    if (!isInCheck(player))
        return false;

    Colour colour = colourOf(player);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            Piece* piece = grid[row][col];
            if (!piece || piece->getColour() != colour)
                continue;

            std::vector<Position> moves = piece->getValidMoves(*this);
            const Position from = {col, row};
            const Position saved = piece->getPosition();

            for (const Position& to : moves) {
                Piece* captured = grid[to.y][to.x];

                const_cast<Board*>(this)->grid[to.y][to.x] = piece;
                const_cast<Board*>(this)->grid[from.y][from.x] = nullptr;
                piece->setPosition(to);

                bool stillInCheck = isInCheck(player);

                const_cast<Board*>(this)->grid[from.y][from.x] = piece;
                const_cast<Board*>(this)->grid[to.y][to.x] = captured;
                piece->setPosition(saved);

                if (!stillInCheck)
                    return false;
            }
        }
    }
    return true;
}

Colour Board::colourOf(Player p)
{
    return (p == Player::White) ? Colour::White : Colour::Black;
}

Player Board::opponentOf(Player p)
{
    return (p == Player::White) ? Player::Black : Player::White;
}

} // namespace chess