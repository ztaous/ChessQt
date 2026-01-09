#pragma once

#include <QObject>
#include <iostream>
#include <vector>

#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "piece.h"
#include "queen.h"
#include "rook.h"

enum class Player
{
    White,
    Black
};

enum class GameMode
{
    standard,
    practice
};

namespace chess
{

class Board : public QObject
{
    Q_OBJECT

  public:
    static constexpr int rows = 8;
    static constexpr int columns = 8;

    explicit Board(QObject* parent = nullptr);
    ~Board();

    void setupBoard(int scenario);
    void cleanBoard();

    bool isPositionValid(Position pos) const;
    bool isOccupied(Position pos) const;
    bool isOppositePiece(const Piece* piece1, const Piece* piece2) const;

    Piece* getPiece(const Position& pos) const;
    void addPiece(const std::string& notation, Piece* piece);
    Position convertNotationToGrid(const std::string& notation);
    void removePiece(const Position& pos);

    bool movePiece(Position current, Position destination);

    void switchPlayer();
    bool canMove(Player player) const;

    std::vector<Position> legalMovesFrom(Position from);
    Position findKing(Player player) const;
    bool isInCheck(Player player) const;
    bool isCheckmate(Player player) const;

    Player getCurrentPlayer() const
    {
        return currentPlayer_;
    }
    void setCurrentPlayer(Player p)
    {
        currentPlayer_ = p;
        emit playerChanged(currentPlayer_);
    }

    GameMode getGameMode() const
    {
        return currentGameMode_;
    }
    void setGameMode(GameMode m)
    {
        currentGameMode_ = m;
    }

    static Colour colourOf(Player p);
    static Player opponentOf(Player p);

  signals:
    void playerChanged(Player);
    void gameOver(Player);

  private:
    Piece* grid[rows][columns]{};
    Player currentPlayer_ = Player::White;
    GameMode currentGameMode_ = GameMode::standard;
};

} // namespace chess
