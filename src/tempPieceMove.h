#pragma once
#include <vector>
#include "piece.h"
#include "board.h"

namespace chess {

class TempPieceMove {
public:
    TempPieceMove(Piece* piece, const Position& newPosition)
        : piece_(piece), savedPosition_(piece->getPosition())
    { piece_->setPosition(newPosition); }

    ~TempPieceMove() { piece_->setPosition(savedPosition_); }

    TempPieceMove(const TempPieceMove&) = delete;
    TempPieceMove& operator=(const TempPieceMove&) = delete;

private:
    Piece*   piece_;
    Position savedPosition_;
};

}
