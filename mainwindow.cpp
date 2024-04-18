#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "chess_logic/piece.h"
#include "chess_logic/board.h"
#include "chess_logic/king.h"
#include "chess_logic/rook.h"
#include "chess_logic/bishop.h"


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

void MainWindow::loadIcons()
{
    pieceIcons["BlackKing"] = QIcon("../images/Chess_kdt45.svg");
    pieceIcons["WhiteKing"] = QIcon("../images/Chess_klt45.svg");
    pieceIcons["BlackBishop"] = QIcon("../images/Chess_bdt45.svg");
    pieceIcons["WhiteBishop"] = QIcon("../images/Chess_blt45.svg");
    pieceIcons["BlackRook"] = QIcon("../images/Chess_rdt45.svg");
    pieceIcons["WhiteRook"] = QIcon("../images/Chess_rlt45.svg");
}

void MainWindow::prepareBoard()
{
    for (int rows = 0; rows < 8; rows++) {
        for (int columns = 0; columns < 8; columns++) {
            QPushButton* button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(rows, columns)->widget());
            connect(button, &QPushButton::clicked, this, [this, rows, columns]() { this->squareClicked(rows, columns); });
        }
    }
}


void MainWindow::squareClicked(int rows, int columns)
{
    QPushButton* button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(rows, columns)->widget());

    // changer la couleur des cases dispoonibles a la piece selectionnee
    
    chess::Position position{columns, rows};
    chess::Piece* piece = board->getPiece(position);

    if (piece != nullptr) {
        // continuer la logique du mouvement
    }

    this->refreshBoard();
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
                button->setIconSize(button->size());

            } else {
                button->setIcon(QIcon());
            }
        }
    }
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

