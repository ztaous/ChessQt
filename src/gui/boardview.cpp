#include "boardview.h"
#include <QMouseEvent>
#include <QSvgRenderer>
#include <QPainter>
#include <QDebug>

BoardView::BoardView(chess::Board* board, QWidget* parent): QGraphicsView(parent), scene(new QGraphicsScene(this)), board(board)
{
    setScene(scene);
    scene->setSceneRect(0, 0, squareSize * chess::Board::columns, squareSize * chess::Board::rows);

    initializeBoard();
    updateBoard();
}

void BoardView::initializeBoard()
{
    loadPieceIcons();

    const QColor light(230, 237, 245);
    const QColor dark(113, 140, 173);

    for (int row = 0; row < chess::Board::rows; ++row) {
        for (int col = 0; col < chess::Board::columns; ++col) {
            auto* square = new QGraphicsRectItem(col * squareSize, row * squareSize, squareSize, squareSize);
            square->setBrush(((row + col) % 2 == 0) ? light : dark);
            square->setPen(Qt::NoPen);
            scene->addItem(square);
            squares[row][col] = square;
        }
    }
}

void BoardView::loadPieceIcons()
{
    pieceIcons.clear();
    pieceIcons["BlackKing"]   = loadAndRenderSvg(basePath + "/Chess_kdt45.svg", squareSize);
    pieceIcons["WhiteKing"]   = loadAndRenderSvg(basePath + "/Chess_klt45.svg", squareSize);
    pieceIcons["BlackQueen"]  = loadAndRenderSvg(basePath + "/Chess_qdt45.svg", squareSize);
    pieceIcons["WhiteQueen"]  = loadAndRenderSvg(basePath + "/Chess_qlt45.svg", squareSize);
    pieceIcons["BlackBishop"] = loadAndRenderSvg(basePath + "/Chess_bdt45.svg", squareSize);
    pieceIcons["WhiteBishop"] = loadAndRenderSvg(basePath + "/Chess_blt45.svg", squareSize);
    pieceIcons["BlackRook"]   = loadAndRenderSvg(basePath + "/Chess_rdt45.svg", squareSize);
    pieceIcons["WhiteRook"]   = loadAndRenderSvg(basePath + "/Chess_rlt45.svg", squareSize);
    pieceIcons["BlackKnight"] = loadAndRenderSvg(basePath + "/Chess_ndt45.svg", squareSize);
    pieceIcons["WhiteKnight"] = loadAndRenderSvg(basePath + "/Chess_nlt45.svg", squareSize);
    pieceIcons["BlackPawn"]   = loadAndRenderSvg(basePath + "/Chess_pdt45.svg", squareSize);
    pieceIcons["WhitePawn"]   = loadAndRenderSvg(basePath + "/Chess_plt45.svg", squareSize);
}

QPixmap BoardView::loadAndRenderSvg(const QString& filePath, int size) const
{
    QSvgRenderer svgRenderer(filePath);
    if (!svgRenderer.isValid()) {
        qWarning() << "Failed to load SVG:" << filePath;
        return QPixmap();
    }

    const qreal dpr = devicePixelRatioF();
    QPixmap pixmap(size, size);
    pixmap.setDevicePixelRatio(dpr);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    svgRenderer.render(&painter);

    return pixmap;
}

void BoardView::setPieceStyle(const QString& style)
{
    basePath = "resources/chess_icons/" + style;
    loadPieceIcons();
    updateBoard();
}

std::unique_ptr<QGraphicsPixmapItem> BoardView::createPiece(const QString& key, int row, int col)
{
    auto item = std::make_unique<QGraphicsPixmapItem>();
    const QPixmap pixmap = pieceIcons.value(key);
    item->setPixmap(pixmap);
    item->setScale(1.0);

    const qreal xOffset = (squareSize - pixmap.width())  / 2.0;
    const qreal yOffset = (squareSize - pixmap.height()) / 2.0;
    item->setPos(col * squareSize + xOffset, row * squareSize + yOffset);

    return item;
}

void BoardView::clearAllPieces()
{
    for (int row = 0; row < chess::Board::rows; ++row) {
        for (int col = 0; col < chess::Board::columns; ++col) {
            if (pieceItems[row][col]) {
                scene->removeItem(pieceItems[row][col].get());
                pieceItems[row][col].reset();
            }
        }
    }
}

void BoardView::updateBoard()
{
    clearAllPieces();

    for (int row = 0; row < chess::Board::rows; ++row) {
        for (int col = 0; col < chess::Board::columns; ++col) {
            chess::Piece* piece = board->getPiece({col, row});
            if (piece) {
                const QString key = QString::fromStdString(
                    (piece->isBlack() ? std::string("Black") : std::string("White")) + piece->typeAsString()
                );
                pieceItems[row][col] = createPiece(key, row, col);
                scene->addItem(pieceItems[row][col].get());
            }
        }
    }
}

chess::Position BoardView::getBoardPosition(const QPoint& viewPos) const
{
    const int x = viewPos.x() / squareSize;
    const int y = viewPos.y() / squareSize;
    return {x, y};
}

void BoardView::mousePressEvent(QMouseEvent* event)
{
    const chess::Position boardPos = getBoardPosition(event->pos());

    if (board && board->isPositionValid(boardPos)) {
        emit squareClicked(boardPos);
    }
    QGraphicsView::mousePressEvent(event);
}

void BoardView::showSelection(chess::Position pos)
{
    if (!board || !board->isPositionValid(pos) || !scene)
        return;

    const QRectF r(pos.x * squareSize, pos.y * squareSize, squareSize, squareSize);

    if (!selectionItem) {
        selectionItem = scene->addRect(
            r,
            QPen(QColor(255, 215, 0, 200), 2.0),
            QBrush(QColor(255, 215, 0, 60))
        );
        selectionItem->setZValue(1000);
    } else {
        selectionItem->setRect(r);
        selectionItem->setVisible(true);
    }
}

void BoardView::clearSelection()
{
    if (selectionItem) {
        selectionItem->setVisible(false);
    }
}
