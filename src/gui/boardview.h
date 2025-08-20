#pragma once

#include <memory>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QMap>

#include "../board.h"

class BoardView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BoardView(chess::Board* board, QWidget* parent = nullptr);
    void updateBoard();
    void setPieceStyle(const QString& style);

signals:
    void squareClicked(chess::Position pos);

public slots:
    void showSelection(chess::Position pos);
    void clearSelection();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void initializeBoard();
    void loadPieceIcons();
    QPixmap loadAndRenderSvg(const QString &filePath, int size) const;
    void clearAllPieces();
    std::unique_ptr<QGraphicsPixmapItem> createPiece(const QString& key, int row, int col);
    chess::Position getBoardPosition(const QPoint& viewPos) const;

private:
    QGraphicsScene* scene = nullptr;
    chess::Board*   board = nullptr;

    static constexpr int squareSize = 75;
    QGraphicsRectItem* squares[chess::Board::rows][chess::Board::columns] = {};
    std::unique_ptr<QGraphicsPixmapItem> pieceItems[chess::Board::rows][chess::Board::columns] = {};
    QMap<QString, QPixmap> pieceIcons;

    QString basePath = "resources/chess_icons/default";
    QGraphicsRectItem* selectionItem { nullptr };
};
