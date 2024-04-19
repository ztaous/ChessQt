#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "piece.h"
#include "board.h"
#include "king.h"
#include "rook.h"
#include "bishop.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), board(new chess::Board())
{
    ui->setupUi(this);

    loadIcons();
    prepareBoard();
    refreshBoard();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete board;
}


QString MainWindow::pieceTypeToString(chess::PieceType type)
{
    switch (type) {
        case chess::PieceType::King:   
        return "King";
        
        case chess::PieceType::Queen:  
        return "Queen";

        case chess::PieceType::Bishop: 
        return "Bishop";

        case chess::PieceType::Knight:
        return "Knight";

        case chess::PieceType::Rook:   
        return "Rook";

        case chess::PieceType::Pawn:   
        return "Pawn";

        default:                
        return "noType";
    }
}


void MainWindow::loadIcons()
{
    pieceIcons["BlackKing"] = QIcon("images/Chess_kdt45.svg");
    pieceIcons["WhiteKing"] = QIcon("images/Chess_klt45.svg");
    pieceIcons["BlackBishop"] = QIcon("images/Chess_bdt45.svg");
    pieceIcons["WhiteBishop"] = QIcon("images/Chess_blt45.svg");
    pieceIcons["BlackRook"] = QIcon("images/Chess_rdt45.svg");
    pieceIcons["WhiteRook"] = QIcon("images/Chess_rlt45.svg");
}


void MainWindow::prepareBoard()
{
    for (int rows = 0; rows < 8; rows++) {
        for (int columns = 0; columns < 8; columns++) {
            QPushButton* button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(rows, columns)->widget());
            connect(button, &QPushButton::clicked, this, [this, rows, columns]() { this->squareClicked(rows, columns); });
        }
    }

    QPushButton* resetButton = qobject_cast<QPushButton*>(ui->resetButton);
    connect(resetButton, &QPushButton::clicked, this, [this]() { this->resetClicked(); });
}


void MainWindow::squareClicked(int rows, int columns)
{
    QPushButton* button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(rows, columns)->widget());
    
    chess::Position position{columns, rows};
    chess::Piece* piece = board->getPiece(position);
    
    if (pieceSelected == false && piece != nullptr) {
        clickedPosition = position;
        pieceSelected = true;
    }
    
    else if (pieceSelected == true) {
        if (board->movePiece(clickedPosition, position) == true) {
            pieceSelected = false;
            refreshBoard();
        }
        else {
            pieceSelected = false;
        }
    }

    refreshBoard();
}


void MainWindow::refreshBoard()
{   
    for (int rows = 0; rows < 8; rows++) {
        for (int columns = 0; columns < 8; columns++) {
            QPushButton* button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(rows, columns)->widget());
            chess::Position position{columns, rows};
            chess::Piece* piece = board->getPiece(position);

            if (piece != nullptr) {
                QString key = (piece->isBlack() ? "Black" : "White") + pieceTypeToString(piece->type());
                button->setIcon(pieceIcons[key]);
                button->setIconSize(QSize(67, 67));

            } else {
                button->setIcon(QIcon());
            }
        }
    }
}


void MainWindow::resetClicked()
{
    delete board;
    board = new chess::Board();
    refreshBoard();
}