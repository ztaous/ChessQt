#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../board.h"
#include "boardview.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), board(new chess::Board())
{
    ui->setupUi(this);

    boardView = new BoardView(board, ui->graphicsViewBoard);

    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::resetGame);
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);

    connect(ui->menuPositions->actions().at(1), &QAction::triggered, this, [this](){ selectScenario(1); });
    connect(ui->menuPositions->actions().at(2), &QAction::triggered, this, [this](){ selectScenario(2); });
    connect(ui->menuPositions->actions().at(3), &QAction::triggered, this, [this](){ selectScenario(3); });

    updateGameStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete board;
}

void MainWindow::selectScenario(int scenario)
{
    savedScenario = scenario;
    board->setupBoard(scenario);
    board->setCurrentPlayer(Player::White);
    boardView->updateBoard();
    ui->gameStatus->setText("White to move");
}

void MainWindow::resetGame()
{
    delete board;
    board = new chess::Board();
    
    // selectScenario(savedScenario);

    boardView->updateBoard();
}

void MainWindow::updateGameStatus()
{
    QString status = (board->getCurrentPlayer() == Player::White) ? "White to move" : "Black to move";
    ui->gameStatus->setText(status);    
}
