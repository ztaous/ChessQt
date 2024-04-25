#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QIcon>
#include <QMap>
#include <QTimer>

#include "board.h"
#include "piece.h"


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

private:
    Ui::MainWindow *ui;
    chess::Board* board;

    QMap<QString, QIcon> pieceIcons;
    QString pieceTypeToString(chess::PieceType type);

    void loadIcons();
    void prepareBoard();
    void squareClicked(int row, int col);
    void refreshBoard(); 
    void resetClicked();
    void flashSquareRed(QPushButton* button);

    bool pieceSelected = false;
    QPushButton* clickedButton; 
    chess::Position clickedPosition;
    QString buttonColor;
};
#endif // MAINWINDOW_H
