#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QPushButton>
#include <QTime>
#include "road.h"
#include "howto.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum GameState {
        PAUSED, RUN, GAMEOVER, WIN, MENU
    };

    enum Level {
        ZEN=0, ONE=1, TWO=2, THREE=3
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event);

public slots:
    // check every intersection of every road for overlapping cars, and update the game accordingly
    void checkCollisions();
    // remove one second from the countdown timer
    void updateCountdown();
    // update score, countdown
    void updateUI();

    // menu button slots
    void startLevelOne();
    void startLevelTwo();
    void startLevelThree();
    void startLevelZen();
    void quit();
    void incLevel();
    void restart();
    void menu();
    void openHowTo();
private:
    Ui::MainWindow *ui;

    QTimer* gameTimer;
    QTimer* countdownTimer;

    QTime remainingTime;

    GameState state;
    Level level;
    HowTo* howTo;
    int targetScore;

    // prepare the roads for a new level
    void init(Level level);
    // update all game parameters
    void updateGameState(GameState state);
};
#endif // MAINWINDOW_H
