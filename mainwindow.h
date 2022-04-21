#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPushButton>
#include <QTime>
#include "road.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum State {
        PAUSED, RUN, GAMEOVER
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event);

public slots:
    void checkCollisions();
    void updateGameState();
    void updateCountdown();
    void updateUI();

private:
    Ui::MainWindow *ui;

    QTimer* gameTimer;
    QTimer* countdownTimer;

    QTime remainingTime;

    State state;
};
#endif // MAINWINDOW_H
