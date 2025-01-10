#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QIcon>
#include <QMap>
#include <QTimer>
#include <QStatusBar>
#include <QLabel>

#include <QMainWindow>
#include "../board.h"
#include "boardview.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void resetGame();
    void selectScenario(int scenario);

private:
    Ui::MainWindow *ui;
    chess::Board* board;
    BoardView* boardView;

    void setGameMode(GameMode mode);
    void updateGameStatus();
    int savedScenario = 0;
};
#endif // MAINWINDOW_H
