#pragma once

#include <memory>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QMap>

#include "../board.h"

class BoardView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit BoardView(chess::Board* board, QGraphicsView* view = nullptr);
    void updateBoard();
    

signals:
    void pieceMoved(const chess::Position& from, const chess::Position& to);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void initializeBoard();
    void loadPieceIcons();
    void loadSvgIcon(const QString &key, const QString &filePath, int size);
    void clearAllPieces();

    void adjustPositionForScale(QGraphicsPixmapItem* item, qreal initScale, qreal finalScale);
    std::unique_ptr<QGraphicsPixmapItem> createPiece(const QString &pieceType, int row, int col);
    QPixmap renderSvg(const QString& pieceType, int targetSize) const;
    chess::Position getBoardPosition(const QPoint &viewPos) const;

    void selectPiece(const chess::Position& pos);
    void deselectPiece();

    QGraphicsScene* scene;
    chess::Board* board;

    static constexpr int squareSize = 75;
    QGraphicsRectItem* squares[chess::Board::rows][chess::Board::columns] = {};
    std::unique_ptr<QGraphicsPixmapItem> pieceItems[chess::Board::rows][chess::Board::columns] = {};
    QMap<QString, QPixmap> pieceIcons;

    chess::Position selectedPiece{-1, -1}; // piece selectionnée (-1, -1 par défaut)
    QGraphicsPixmapItem* selectedPieceItem = nullptr;
};
