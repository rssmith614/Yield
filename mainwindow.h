#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
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
        PAUSED, RUN, GAMEOVER, WIN
    };

    enum Level {
        ONE=1, TWO=2, THREE=3
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
    Level level;

    int targetScore;

    void init();
};
#endif // MAINWINDOW_H
