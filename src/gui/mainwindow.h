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

#include "../piece.h"
#include "../board.h"
#include "../king.h"
#include "../rook.h"
#include "../bishop.h"
#include "../knight.h"
#include "../queen.h"

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
    void resetGame();
    void selectScenario(int scenario);
    // void squareClick(int row, int col);

private:
    Ui::MainWindow *ui;
    chess::Board* board;
    BoardView* boardView;

    // QMap<QString, QIcon> pieceIcons;
    // QString pieceTypeToString(chess::PieceType type);

    // void loadImages();
    // void prepareBoard();
    // void refreshBoard(); 
    // void flashSquareRed(QPushButton* button);
    void updateGameStatus();

    // bool pieceSelected = false;
    // QPushButton* clickedButton; 
    // chess::Position clickedPosition;
    // QString buttonColor;
    int savedScenario = 0;
};
#endif // MAINWINDOW_H
