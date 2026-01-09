#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "boardview.h"
#include <QAction>
#include <QString>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), board(new chess::Board())
{
    ui->setupUi(this);

    boardView = new BoardView(board, ui->graphicsViewBoard);

    connect(boardView, &BoardView::squareClicked, this, &MainWindow::onBoardSquareClicked);

    connect(ui->actionNewGame, &QAction::triggered, this, &MainWindow::newGame);
    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::resetGame);
    connect(ui->actionPracticeBoard, &QAction::triggered, this, [this]() { setGameMode(GameMode::practice); });
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);

    connect(board, &chess::Board::playerChanged, this, &MainWindow::updateGameStatus);
    connect(board, &chess::Board::gameOver, this, &MainWindow::endGame);

    connect(ui->menuPositions->actions().at(1), &QAction::triggered, this, [this]() { selectScenario(1); });
    connect(ui->menuPositions->actions().at(2), &QAction::triggered, this, [this]() { selectScenario(2); });
    connect(ui->menuPositions->actions().at(3), &QAction::triggered, this, [this]() { selectScenario(3); });

    updateGameStatus();
}

MainWindow::~MainWindow()
{
    delete board;
    delete ui;
}

void MainWindow::newGame()
{
    setGameMode(GameMode::standard);
    selectScenario(0); // default classic position
}

void MainWindow::endGame(Player winner)
{
    const char* who = (winner == Player::White) ? "White" : "Black";
    ui->gameStatus->setText(QString("Checkmate - %1 wins").arg(who));
}

void MainWindow::resetGame()
{
    board->setupBoard(savedScenario);
    board->setCurrentPlayer(Player::White);
    refreshBoard();
}

void MainWindow::selectScenario(int scenario)
{
    savedScenario = scenario;
    board->setupBoard(scenario);
    board->setCurrentPlayer(Player::White);
    refreshBoard();
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
    const char* who = (board->getCurrentPlayer() == Player::White) ? "White" : "Black";
    ui->gameStatus->setText(QString("%1 to move").arg(who));
}

void MainWindow::onBoardSquareClicked(chess::Position pos)
{
    if (selectingFrom.x == -1 && selectingFrom.y == -1) {
        chess::Piece* p = board->getPiece(pos);
        if (!p)
            return;

        const bool pieceIsWhite = !p->isBlack();
        const bool whiteToMove = (board->getCurrentPlayer() == Player::White);

        if ((whiteToMove && !pieceIsWhite) || (!whiteToMove && pieceIsWhite)) {
            return;
        }

        selectingFrom = pos;
        boardView->showSelection(pos);

        std::vector<chess::Position> moves;
        moves = board->legalMovesFrom(pos);

        std::vector<chess::Position> quiet, caps;
        quiet.reserve(moves.size());
        caps.reserve(moves.size());

        for (const auto& to : moves) {
            if (auto* t = board->getPiece(to)) {
                if (t->isBlack() != p->isBlack())
                    caps.push_back(to);
                else
                    quiet.push_back(to);
            } else {
                quiet.push_back(to);
            }
        }

        boardView->showMoveHints(quiet, caps);
        return;
    }

    const chess::Position from = selectingFrom;
    selectingFrom = {-1, -1};
    boardView->clearSelection();
    boardView->clearMoveHints();

    if (board->movePiece(from, pos)) {
        refreshBoard();
    }
}

void MainWindow::refreshBoard()
{
    boardView->updateBoard();

    const Player side = board->getCurrentPlayer();
    const chess::Position kingPos = board->findKing(side);

    if (kingPos.x != -1 && kingPos.y != -1 && board->isInCheck(side)) {
        boardView->showKingCheck(kingPos);
    } else {
        boardView->clearKingCheck();
    }

    updateGameStatus();
}
