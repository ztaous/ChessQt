#include <QMouseEvent>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QDebug>
#include <QSvgRenderer>

#include "boardview.h"

namespace
{
    constexpr qreal INITIAL_SCALE = 1.0; // scale initial des pieces
    constexpr qreal SELECTED_SCALE = 1.15; // scale lorsque la piece est selectionné
}


BoardView::BoardView(chess::Board* board, QGraphicsView* view) : QGraphicsView(view), scene(new QGraphicsScene(this)), board(board)
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing, false);
    setRenderHint(QPainter::SmoothPixmapTransform, false);
    scene->setSceneRect(0, 0, view->width(), view->height());

    initializeBoard();
    updateBoard();
}

void BoardView::initializeBoard()
{
    loadPieceIcons();

    QColor light(230, 237, 245);
    QColor dark(113, 140, 173);

    for (int row = 0; row < chess::Board::rows; row++) {
        for (int col = 0; col < chess::Board::columns; col++) {
            QGraphicsRectItem* square = new QGraphicsRectItem(col * squareSize, row * squareSize, squareSize, squareSize);
            
            square->setBrush((row + col) % 2 == 0 ? dark : light);
            square->setPen(Qt::NoPen); // enleve les bordures
            scene->addItem(square);
            squares[row][col] = square;
        }
    }
}

void BoardView::loadPieceIcons()
{
    QString basePath = "resources/chess_icons/";
    int targetSize = squareSize; // icone aura la même taille que la case

    loadSvgIcon("BlackKing", basePath + "Chess_kdt45.svg", targetSize);
    loadSvgIcon("WhiteKing", basePath + "Chess_klt45.svg", targetSize);
    loadSvgIcon("BlackQueen", basePath + "Chess_qdt45.svg", targetSize);
    loadSvgIcon("WhiteQueen", basePath + "Chess_qlt45.svg", targetSize);
    loadSvgIcon("BlackBishop", basePath + "Chess_bdt45.svg", targetSize);
    loadSvgIcon("WhiteBishop", basePath + "Chess_blt45.svg", targetSize);
    loadSvgIcon("BlackRook", basePath + "Chess_rdt45.svg", targetSize);
    loadSvgIcon("WhiteRook", basePath + "Chess_rlt45.svg", targetSize);
    loadSvgIcon("BlackKnight", basePath + "Chess_ndt45.svg", targetSize);
    loadSvgIcon("WhiteKnight", basePath + "Chess_nlt45.svg", targetSize);
    loadSvgIcon("BlackPawn", basePath + "Chess_pdt45.svg", targetSize);
    loadSvgIcon("WhitePawn", basePath + "Chess_plt45.svg", targetSize);
}

QPixmap BoardView::renderSvg(const QString& filePath, int targetSize) const
{
    QSvgRenderer svgRenderer(filePath);

    QPixmap pixmap(targetSize, targetSize);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    svgRenderer.render(&painter);

    return pixmap;
}

std::unique_ptr<QGraphicsPixmapItem> BoardView::createPiece(const QString& pieceType, int row, int col)
{
    auto item = std::make_unique<QGraphicsPixmapItem>();

    QPixmap pixmap = pieceIcons[pieceType];
    item->setPixmap(pixmap);
    item->setScale(INITIAL_SCALE);

    qreal xOffset = (squareSize - pixmap.width() * INITIAL_SCALE) / 2;
    qreal yOffset = (squareSize - pixmap.height() * INITIAL_SCALE) / 2;
    item->setPos(col * squareSize + xOffset, row * squareSize + yOffset);

    return item;
}


void BoardView::adjustPositionForScale(QGraphicsPixmapItem* item, qreal initScale, qreal finalScale)
{
    qreal adjustment = (finalScale - initScale) * squareSize / 2.0;
    item->setPos(item->x() - adjustment, item->y() - adjustment);
}

void BoardView::clearAllPieces()
{
    for (int row = 0; row < chess::Board::rows; row++) {
        for (int col = 0; col < chess::Board::columns; col++) {
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

    for (int row = 0; row < chess::Board::rows; row++) {
        for (int col = 0; col < chess::Board::columns; col++) {
            chess::Piece* piece = board->getPiece({col, row});

            if (piece) {
                QString key = QString::fromStdString((piece->isBlack() ? "Black" : "White") + piece->typeAsString());
                pieceItems[row][col] = createPiece(key, row, col);
                scene->addItem(pieceItems[row][col].get());
            }
        }
    }
}

void BoardView::loadSvgIcon(const QString& key, const QString& filePath, int size)
{
    QSvgRenderer svgRenderer(filePath);
    if (!svgRenderer.isValid()) {
        qWarning() << "Failed to load SVG:" << filePath;
        return;
    }

    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    pieceIcons[key] = pixmap;
}

void BoardView::selectPiece(const chess::Position& pos)
{
    deselectPiece();

    selectedPiece = pos;

    if (pieceItems[pos.y][pos.x]) {
        selectedPieceItem = pieceItems[pos.y][pos.x].get();
        adjustPositionForScale(selectedPieceItem, INITIAL_SCALE, SELECTED_SCALE);
        selectedPieceItem->setScale(SELECTED_SCALE);
    }
}

void BoardView::deselectPiece()
{
    if (selectedPieceItem) {
        adjustPositionForScale(selectedPieceItem, SELECTED_SCALE, INITIAL_SCALE);
        selectedPieceItem->setScale(INITIAL_SCALE);
        selectedPieceItem = nullptr;
    }
    selectedPiece = {-1, -1};
}

void BoardView::mousePressEvent(QMouseEvent* event)
{
    QPoint clickPos = event->pos();
    chess::Position boardPos = getBoardPosition(clickPos);

    if (!board->isPositionValid(boardPos)) {
        return;
    }
    if (selectedPiece.x == -1 && selectedPiece.y == -1) {
        if (board->getPiece(boardPos)) {
            selectPiece(boardPos);
        }
    }
    else {
        if (board->movePiece(selectedPiece, boardPos)) {
            deselectPiece();
            updateBoard();
        }
        deselectPiece();
    }

    QGraphicsView::mousePressEvent(event);
}

chess::Position BoardView::getBoardPosition(const QPoint& viewPos) const
{
    int x = viewPos.x() / squareSize;
    int y = viewPos.y() / squareSize;
    return {x, y};
}
