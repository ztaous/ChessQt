#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../board.h"
#include "boardview.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), board(new chess::Board())
{
    ui->setupUi(this);
    boardView = new BoardView(board, ui->graphicsViewBoard);

    connect(ui->actionNewGame, &QAction::triggered, this, &MainWindow::newGame);
    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::resetGame);
    connect(ui->actionPracticeBoard, &QAction::triggered, this, [this]() { setGameMode(GameMode::practice); });
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);
    
    connect(board, &chess::Board::playerChanged, this, &MainWindow::updateGameStatus);
    connect(board, &chess::Board::gameOver, this, &MainWindow::endGame);

    connect(ui->menuPositions->actions().at(1), &QAction::triggered, this, [this](){ selectScenario(1); });
    connect(ui->menuPositions->actions().at(2), &QAction::triggered, this, [this](){ selectScenario(2); });
    connect(ui->menuPositions->actions().at(3), &QAction::triggered, this, [this](){ selectScenario(3); });

    updateGameStatus();
}

MainWindow::~MainWindow()
{
    delete boardView;
    delete board;
    delete ui;
}

void MainWindow::newGame()
{
    setGameMode(GameMode::standard);
    selectScenario(0);  // default classic position
}

void MainWindow::endGame(Player winner)
{
    QString winnerName = (winner == Player::White) ? "White" : "Black";
    ui->gameStatus->setText("CHECKMATE - " + winnerName + " wins");
}

void MainWindow::resetGame()
{    
    board->setupBoard(savedScenario);
    board->setCurrentPlayer(Player::White);
    boardView->updateBoard();
    updateGameStatus();
}

void MainWindow::selectScenario(int scenario)
{
    savedScenario = scenario;
    board->setupBoard(scenario);
    board->setCurrentPlayer(Player::White);
    boardView->updateBoard();
    updateGameStatus();
}

void MainWindow::setGameMode(GameMode mode)
{
    board->setGameMode(mode);
    updateGameStatus();
}

void MainWindow::updateGameStatus()
{
    if (board->getGameMode() == GameMode::practice) {
        ui->gameStatus->setText("Practice mode");
        return;
    }
    
    QString status = (board->getCurrentPlayer() == Player::White) ? "White to move" : "Black to move";
    ui->gameStatus->setText(status);    
}
