#pragma once

#include <iostream>
#include <vector>
#include <QObject>

#include "piece.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"


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


namespace chess {

class Board : public QObject
{
    Q_OBJECT

public:
    explicit Board(QObject* parent = nullptr);

    ~Board();
    void cleanBoard();
    void setupBoard(int scenario);
      
    bool isPositionValid(Position pos) const;
    bool isOccupied(Position pos) const;
    bool isOppositePiece(const Piece* piece1, const Piece* piece2) const;
   
    Piece* getPiece(const Position& pos) const;
    bool movePiece(Position current, Position destination);

    Player getCurrentPlayer() const { return currentPlayer_; }
    void setCurrentPlayer(const Player& player) { currentPlayer_ = player; }
    void setGameMode(const GameMode& mode) { currentGameMode_ = mode; }
    GameMode getGameMode() { return currentGameMode_; };

    static const int rows = 8; 
    static const int columns = 8; 
    
signals:
    void pieceMoved(const Position& current, const Position& destination);
    void playerChanged(Player currentPlayer);
    
private:
    void switchPlayer();
    bool canMove(Player player) const;

    Piece* grid[rows][columns];
    Player currentPlayer_ = Player::White;
    GameMode currentGameMode_ = GameMode::standard;
};

}
