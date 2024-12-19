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
    void setPieceStyle(const QString& style);
    
signals:
    void pieceMoved(const chess::Position& from, const chess::Position& to);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void initializeBoard();
    void loadPieceIcons();
    QPixmap loadAndRenderSvg(const QString &filePath, int size) const;
 
    void clearAllPieces();
    std::unique_ptr<QGraphicsPixmapItem> createPiece(const QString &pieceType, int row, int col);
    chess::Position getBoardPosition(const QPoint &viewPos) const;

    void selectPiece(const chess::Position& pos);
    void deselectPiece();

    QGraphicsScene* scene;
    chess::Board* board;

    static constexpr int squareSize = 75;
    QGraphicsRectItem* squares[chess::Board::rows][chess::Board::columns] = {};
    std::unique_ptr<QGraphicsPixmapItem> pieceItems[chess::Board::rows][chess::Board::columns] = {};
    QMap<QString, QPixmap> pieceIcons;

    QString basePath = "resources/chess_icons/default";
    QColor originalColor;
    chess::Position selectedPiece{-1, -1}; // piece selectionnée (-1, -1 par défaut)
};
