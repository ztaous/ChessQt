#include <QMouseEvent>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QDebug>
#include <QSvgRenderer>

#include "boardview.h"


BoardView::BoardView(chess::Board* board, QGraphicsView* view) : QGraphicsView(view), scene(new QGraphicsScene(this)), board(board)
{
    setScene(scene);
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
            square->setPen(Qt::NoPen);
            scene->addItem(square);
            squares[row][col] = square;
        }
    }
}


void BoardView::loadPieceIcons()
{
    pieceIcons.clear();
    
    QString basePath = "resources/chess_icons/";
    int targetSize = squareSize; // icone aura la même taille que la case

    pieceIcons["BlackKing"] = loadAndRenderSvg(basePath + "/Chess_kdt45.svg", targetSize);
    pieceIcons["WhiteKing"] = loadAndRenderSvg(basePath + "/Chess_klt45.svg", targetSize);
    pieceIcons["BlackQueen"] = loadAndRenderSvg(basePath + "/Chess_qdt45.svg", targetSize);
    pieceIcons["WhiteQueen"] = loadAndRenderSvg(basePath + "/Chess_qlt45.svg", targetSize);
    pieceIcons["BlackBishop"] = loadAndRenderSvg(basePath + "/Chess_bdt45.svg", targetSize);
    pieceIcons["WhiteBishop"] = loadAndRenderSvg(basePath + "/Chess_blt45.svg", targetSize);
    pieceIcons["BlackRook"] = loadAndRenderSvg(basePath + "/Chess_rdt45.svg", targetSize);
    pieceIcons["WhiteRook"] = loadAndRenderSvg(basePath + "/Chess_rlt45.svg", targetSize);
    pieceIcons["BlackKnight"] = loadAndRenderSvg(basePath + "/Chess_ndt45.svg", targetSize);
    pieceIcons["WhiteKnight"] = loadAndRenderSvg(basePath + "/Chess_nlt45.svg", targetSize);
    pieceIcons["BlackPawn"] = loadAndRenderSvg(basePath + "/Chess_pdt45.svg", targetSize);
    pieceIcons["WhitePawn"] = loadAndRenderSvg(basePath + "/Chess_plt45.svg", targetSize);
}


QPixmap BoardView::loadAndRenderSvg(const QString& filePath, int size) const
{
    QSvgRenderer svgRenderer(filePath);
    if (!svgRenderer.isValid()) {
        qWarning() << "Failed to load SVG:" << filePath;
        return QPixmap();
    }

    QPixmap pixmap(size, size);
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


std::unique_ptr<QGraphicsPixmapItem> BoardView::createPiece(const QString& pieceType, int row, int col)
{
    auto item = std::make_unique<QGraphicsPixmapItem>();

    QPixmap pixmap = pieceIcons[pieceType];
    item->setPixmap(pixmap);
    item->setScale(1.0);

    qreal xOffset = (squareSize - pixmap.width() * 1.0) / 2;
    qreal yOffset = (squareSize - pixmap.height() * 1.0) / 2;
    item->setPos(col * squareSize + xOffset, row * squareSize + yOffset);

    return item;
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
                QString pieceType = QString::fromStdString((piece->isBlack() ? "Black" : "White") + piece->typeAsString());
                pieceItems[row][col] = createPiece(pieceType, row, col);
                scene->addItem(pieceItems[row][col].get());
            }
        }
    }
}


void BoardView::selectPiece(const chess::Position& pos)
{
    deselectPiece();

    selectedPiece = pos;

    if (squares[pos.y][pos.x]) {
        QGraphicsRectItem* square = squares[pos.y][pos.x];
    
        originalColor = square->brush().color();
        QColor selectedColor(173, 216, 230);
        square->setBrush(selectedColor);
    }
}


void BoardView::deselectPiece()
{
    if (selectedPiece.x != -1 && selectedPiece.y != -1) {
        QGraphicsRectItem* square = squares[selectedPiece.y][selectedPiece.x];
        if (square) {
            square->setBrush(originalColor);
        }
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
        return;
    }

    if (board->movePiece(selectedPiece, boardPos)) {
        deselectPiece();
        updateBoard();
    }
    deselectPiece();

    QGraphicsView::mousePressEvent(event);
}


chess::Position BoardView::getBoardPosition(const QPoint& viewPos) const
{
    int x = viewPos.x() / squareSize;
    int y = viewPos.y() / squareSize;
    return {x, y};
}
