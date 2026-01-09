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
    basePath = ":/chess_icons/" + style;
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
    clearMoveHints();
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
    if (!board || !board->isPositionValid(pos) || !scene) return;

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

void BoardView::showKingCheck(chess::Position pos)
{
    if (!board || !scene) return;

    const QRectF r(pos.x * squareSize, pos.y * squareSize, squareSize, squareSize);
    const QPen border (QColor(180, 20, 20, 110), 1.5);
    const QBrush fill (QColor(200, 20, 20, 28));

    if (!kingCheckItem) {
        kingCheckItem = scene->addRect(r, border, fill);
        kingCheckItem->setZValue(900);
    } else {
        kingCheckItem->setRect(r);
        kingCheckItem->setVisible(true);
    }
}

void BoardView::clearKingCheck()
{
    if (kingCheckItem) kingCheckItem->setVisible(false);
}

void BoardView::showMoveHints(const std::vector<chess::Position>& movement, const std::vector<chess::Position>& captures)
{
    clearMoveHints();

    const qreal rDot   = squareSize * 0.18;
    const qreal rRing  = squareSize * 0.46;
    const qreal center = squareSize * 0.5;

    const QBrush dotBrush(QColor(20, 20, 20, 45));
    QPen ringPen(QColor(40, 40, 40, 60), 3.0);
    ringPen.setCosmetic(true);

    ringPen.setCosmetic(true);
    ringPen.setCapStyle(Qt::RoundCap);
    ringPen.setJoinStyle(Qt::RoundJoin);

    // Movement moves (dot)
    for (const auto& pos : movement) {
        const qreal cx = pos.x * squareSize + center;
        const qreal cy = pos.y * squareSize + center;
        auto* dot = scene->addEllipse(cx - rDot, cy - rDot, 2*rDot, 2*rDot, Qt::NoPen, dotBrush);
        dot->setZValue(200);
        moveHintItems.push_back(dot);
    }

    // Capture moves (ring)
    for (const auto& pos : captures) {
        const qreal x = pos.x * squareSize + (squareSize - 2*rRing) * 0.5;
        const qreal y = pos.y * squareSize + (squareSize - 2*rRing) * 0.5;
        auto* ring = scene->addEllipse(x, y, 2*rRing, 2*rRing, ringPen, Qt::NoBrush);
        ring->setZValue(1100);
        moveHintItems.push_back(ring);
    }
}

void BoardView::clearMoveHints()
{
    for (QGraphicsItem* item : moveHintItems) {
        if (item) scene->removeItem(item);
        delete item;
    }
    moveHintItems.clear();
}
