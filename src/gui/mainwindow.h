#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../board.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class BoardView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void onBoardSquareClicked(chess::Position pos);

private slots:
    void newGame();
    void resetGame();
    void endGame(Player winner);
    void selectScenario(int scenario);

private:
    void setGameMode(GameMode mode);
    void updateGameStatus();

private:
    Ui::MainWindow* ui = nullptr;
    chess::Board*   board = nullptr;
    BoardView*      boardView = nullptr;
    chess::Position selectingFrom { -1, -1 };
    int             savedScenario = 0;
};

#endif // MAINWINDOW_H
