#include <QMouseEvent>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QDebug>
#include <QSvgRenderer>

#include "boardview.h"


BoardView::BoardView(chess::Board* board, QGraphicsView* view) : QGraphicsView(view), scene(new QGraphicsScene(this)), board(board)
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);

    scene->setSceneRect(0, 0, view->width(), view->height());

    initializeBoard();
    updateBoard();
}

void BoardView::initializeBoard()
{
    loadPieceIcons();

    QColor lightBlue(230, 237, 245);
    QColor darkBlue(113, 140, 173);

    for (int row = 0; row < chess::Board::rows; row++) {
        for (int col = 0; col < chess::Board::columns; col++) {
            QGraphicsRectItem* square = new QGraphicsRectItem(col * squareSize, row * squareSize, squareSize, squareSize);
            
            square->setBrush((row + col) % 2 == 0 ? darkBlue : lightBlue);
            
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

std::unique_ptr<QGraphicsPixmapItem> BoardView::createPiece(const QString& pieceType, int row, int col)
{
    auto item = std::make_unique<QGraphicsPixmapItem>(pieceIcons[pieceType]);

    qreal pieceWidth = pieceIcons[pieceType].width() * item->scale();
    qreal pieceHeight = pieceIcons[pieceType].height() * item->scale();
    qreal xOffset = (squareSize - pieceWidth) / 2;
    qreal yOffset = (squareSize - pieceHeight) / 2;

    item->setPos(col * squareSize + xOffset, row * squareSize + yOffset);

    return item;
}

void BoardView::updateBoard()
{
    for (int row = 0; row < chess::Board::rows; row++) {
        for (int col = 0; col < chess::Board::columns; col++) {
            chess::Piece* piece = board->getPiece({col, row});

            if (piece) {
                QString key = QString::fromStdString((piece->isBlack() ? "Black" : "White") + piece->typeAsString());

                if (!pieceItems[row][col]) {
                    pieceItems[row][col] = createPiece(key, row, col);
                    scene->addItem(pieceItems[row][col].get());
                }
            }
            else {
                if (pieceItems[row][col]) {
                    scene->removeItem(pieceItems[row][col].get());
                    pieceItems[row][col].reset();
                }
            }
        }
    }
}

chess::Position BoardView::getBoardPosition(const QPoint& viewPos) const
{
    int x = viewPos.x() / squareSize;
    int y = viewPos.y() / squareSize;
    return {x, y};
}

void BoardView::mousePressEvent(QMouseEvent* event)
{
    QPoint clickPos = event->pos();
    chess::Position boardPos = getBoardPosition(clickPos);

    if (boardPos.x < 0 || boardPos.x >= chess::Board::columns || boardPos.y < 0 || boardPos.y >= chess::Board::rows) {
        return;
    }

    if (selectedPiece.x == -1 && selectedPiece.y == -1) {
        // selectionne la piece
        if (board->getPiece(boardPos)) {
            selectedPiece = boardPos;
            originalColor = squares[boardPos.y][boardPos.x]->brush().color();
            squares[boardPos.y][boardPos.x]->setBrush(Qt::yellow);
        }
    }
    else {
        // bouge la piece
        if (board->movePiece(selectedPiece, boardPos)) {
            updateBoard();
        }
        squares[selectedPiece.y][selectedPiece.x]->setBrush(originalColor);
        selectedPiece = {-1, -1};
    }

    QGraphicsView::mousePressEvent(event);
}
