#pragma once

#include "board.h"

namespace chess
{

// un helper RAII helper qui bouge temporairement une piece
// Retablit l'etat du board quand detruit
class TempMove
{
  public:
    TempMove(Board& board, Position from, Position to)
        : board_(board), from_(from), to_(to), savedPiecePosition_(board.grid[from.y][from.x]->getPosition()),
          captured_(std::move(board.grid[to.y][to.x]))
    {
        board_.grid[to.y][to.x] = std::move(board_.grid[from.y][from.x]);
        board_.grid[to.y][to.x]->setPosition(to);
    }

    ~TempMove()
    {
        board_.grid[to_.y][to_.x]->setPosition(savedPiecePosition_);
        board_.grid[from_.y][from_.x] = std::move(board_.grid[to_.y][to_.x]);
        board_.grid[to_.y][to_.x] = std::move(captured_);
    }

    TempMove(const TempMove&) = delete;
    TempMove& operator=(const TempMove&) = delete;

  private:
    Board& board_;
    Position from_;
    Position to_;
    Position savedPiecePosition_;
    std::unique_ptr<Piece> captured_;
};

} // namespace chess
