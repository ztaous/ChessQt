#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), board(new chess::Board())
{
    ui->setupUi(this);

    ui->gameStatus->setText("Select a position"); 

    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::resetGame);
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);

    connect(ui->menuPositions->actions().at(1), &QAction::triggered, this, [this](){ selectScenario(1); });
    connect(ui->menuPositions->actions().at(2), &QAction::triggered, this, [this](){ selectScenario(2); });
    connect(ui->menuPositions->actions().at(3), &QAction::triggered, this, [this](){ selectScenario(3); });

    loadImages();
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


void MainWindow::loadImages()
{
    pieceIcons["BlackKing"] = QIcon("images/Chess_kdt45.svg");
    pieceIcons["WhiteKing"] = QIcon("images/Chess_klt45.svg");

    pieceIcons["BlackQueen"] = QIcon("images/Chess_qdt45.svg");
    pieceIcons["WhiteQueen"] = QIcon("images/Chess_qlt45.svg");

    pieceIcons["BlackBishop"] = QIcon("images/Chess_bdt45.svg");
    pieceIcons["WhiteBishop"] = QIcon("images/Chess_blt45.svg");

    pieceIcons["BlackRook"] = QIcon("images/Chess_rdt45.svg");
    pieceIcons["WhiteRook"] = QIcon("images/Chess_rlt45.svg");

    pieceIcons["BlackKnight"] = QIcon("images/Chess_ndt45.svg");
    pieceIcons["WhiteKnight"] = QIcon("images/Chess_nlt45.svg");

    pieceIcons["BlackPawn"] = QIcon("images/Chess_pdt45.svg");
    pieceIcons["WhitePawn"] = QIcon("images/Chess_plt45.svg");
}


void MainWindow::selectScenario(int scenario)
{
    savedScenario = scenario;
    board->setupBoard(scenario);

    ui->gameStatus->setText("White to move");

    refreshBoard();
}


void MainWindow::prepareBoard()
{
    for (int rows = 0; rows < 8; rows++) {
        for (int columns = 0; columns < 8; columns++) {
            
            QPushButton* button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(rows, columns)->widget());
            connect(button, &QPushButton::clicked, this, [this, rows, columns](){ this->squareClick(rows, columns); });
        }
    }
}


void MainWindow::squareClick(int rows, int columns)
{
    QPushButton* button = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(rows, columns)->widget());
    chess::Position position{columns, rows};
    chess::Piece* piece = board->getPiece(position);
    
    if (!pieceSelected && piece != nullptr) {
        
        bool isCurrentPlayerBlack = piece->isBlack() && board->getCurrentPlayer() == Player::Black;
        bool isCurrentPlayerWhite = piece->isWhite() && board->getCurrentPlayer() == Player::White;

        if (isCurrentPlayerBlack || isCurrentPlayerWhite) {
            clickedPosition = position;
            clickedButton = button;
            pieceSelected = true;

            buttonColor = clickedButton->styleSheet();
            QString greenColor = "background-color: rgb(95, 158, 160)";
            clickedButton->setStyleSheet(greenColor);
        }
            
    }
    
    else if (pieceSelected) {
        
        if (board->movePiece(clickedPosition, position)) {
            pieceSelected = false;
            clickedButton->setStyleSheet(buttonColor);
            updateGameStatus();
        }
        else {
            flashSquareRed(clickedButton);
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
            } 
            else {
                button->setIcon(QIcon());
            }
        }
    }
}


void MainWindow::resetGame()
{
    delete board;
    board = new chess::Board();

    board->setupBoard(savedScenario);
    ui->gameStatus->setText("White to move");

    refreshBoard();
}


void MainWindow::flashSquareRed(QPushButton* button)
{
    int flashDuration = 500; // ms
    QString redColor = "background-color: rgb(170, 74, 68)";
    
    button->setStyleSheet(redColor);
    button->setEnabled(false);

    QTimer::singleShot(flashDuration, [this, button]()
    { 
        button->setStyleSheet(buttonColor);
        button->setEnabled(true); 
    });
}


void MainWindow::updateGameStatus()
{
    QString colorToPlay = (board->getCurrentPlayer() == Player::White) ? "White to move" : "Black to move";
    ui->gameStatus->setText(colorToPlay);    
}
