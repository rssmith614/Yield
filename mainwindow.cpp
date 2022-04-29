#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set up window
    setWindowTitle("Yield");
    setFixedSize(600,600);

    // game timer - framerate 30fps
    gameTimer = new QTimer();
    gameTimer->start(33);

    // countdown timer - 1s
    countdownTimer = new QTimer();
    countdownTimer->start(1000);

    // set up background - grass + roads
    ui->backgroundWidget->init(ui->RoadA->geometry().y(), ui->RoadA->geometry().height());

    // connect menu buttons to their slots
    connect(ui->nextButton, SIGNAL (pressed()), this, SLOT(incLevel()));
    connect(ui->restartButton, SIGNAL(pressed()), this, SLOT(restart()));
    connect(ui->exitButton, SIGNAL(pressed()), this, SLOT(menu()));

    connect(ui->levelOne, SIGNAL(pressed()), this, SLOT(startLevelOne()));
    connect(ui->levelTwo, SIGNAL(pressed()), this, SLOT(startLevelTwo()));
    connect(ui->levelThree, SIGNAL(pressed()), this, SLOT(startLevelThree()));
    connect(ui->quit, SIGNAL(pressed()), this, SLOT(quit()));

    // these come in pairs
    state = MENU;
    updateGameState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // esc key pause button
    if (event->key() == Qt::Key_Escape) {
        if (state == RUN) {
            state = PAUSED;
        } else if (state == PAUSED){
            state = RUN;
        }
        updateGameState();
    }

    // 'win' button for testing/demo
    if (event->key() == Qt::Key_D) {
        state = WIN;
        updateGameState();
    }
}

void MainWindow::checkCollisions()
{
    // for every car on road 1
    for (Car* car_1 : ui->Road1->cars)
    {
        // define its bounding box
        QRect car_1_bb = Tools::getBoundingBox(car_1, ui->Road1);

        // for every car on road a
        for (Car* car_a : ui->RoadA->cars)
        {
            // define its bounding box
            QRect car_a_bb = Tools::getBoundingBox(car_a, ui->RoadA);

            // check intersection between every car on road 1 and road a
            if (car_1_bb.intersects(car_a_bb) && !(car_1->isCrashed() && car_a->isCrashed())) {
                car_1->notifyCollision();
                car_a->notifyCollision();
                state = GAMEOVER;
                updateGameState();
            }
        }

        // for every car on road b
        for (Car* car_b : ui->RoadB->cars)
        {
            // define its bounding box
            QRect car_b_bb = Tools::getBoundingBox(car_b, ui->RoadB);

            // check intersection between every car on road 1 and road b
            if (car_1_bb.intersects(car_b_bb) && !(car_1->isCrashed() && car_b->isCrashed())) {
                car_1->notifyCollision();
                car_b->notifyCollision();
                state = GAMEOVER;
                updateGameState();
            }
        }
    }

    // for every car on road 2
    for (Car* car_2 : ui->Road2->cars)
    {
        // define its bounding box
        QRect car_2_bb = Tools::getBoundingBox(car_2, ui->Road2);

        // for every car on road a
        for (Car* car_a : ui->RoadA->cars)
        {
            // define its bounding box
            QRect car_a_bb = Tools::getBoundingBox(car_a, ui->RoadA);

            // check intersection between every car on road 2 and road a
            if (car_2_bb.intersects(car_a_bb) && !(car_2->isCrashed() && car_a->isCrashed())) {
                car_2->notifyCollision();
                car_a->notifyCollision();
                state = GAMEOVER;
                updateGameState();
            }
        }

        // for every car on road b
        for (Car* car_b : ui->RoadB->cars)
        {
            // define its bounding box
            QRect car_b_bb = Tools::getBoundingBox(car_b, ui->RoadB);

            // check intersection between every car on road 2 and road b
            if (car_2_bb.intersects(car_b_bb) && !(car_2->isCrashed() && car_b->isCrashed())) {
                car_2->notifyCollision();
                car_b->notifyCollision();
                state = GAMEOVER;
                updateGameState();
            }
        }
    }
}

void MainWindow::init()
{
    // update on-screen level indicator
    ui->levelLabel->setText("Level " + QString::number(level));
    // reset score
    VerticalRoad::clearedCars = 0;

    // remove cars from all roads
    ui->Road1->clear();
    ui->Road2->clear();
    ui->RoadA->clear();
    ui->RoadB->clear();

    // turn on the stop signs (traffic stops by default)
    ui->stopSign1->set(true);
    ui->stopSign2->set(true);

    switch(level) {
    case ONE:

        // define win conditions
        targetScore = 2;
        remainingTime.setHMS(0,1,0);    // 1:00

        // only road 2 is active, so hide the other stop sign
        ui->stopSign1->hide();
        ui->stopSign2->show();
        // define which preset each road made in the ui should have
        ui->RoadA->setPreset(Road::DISABLED, Road::LEFT);
        ui->RoadB->setPreset(Road::A, Road::RIGHT);
        ui->Road1->setPreset(Road::DISABLED, Road::DOWN);
        ui->Road2->setPreset(Road::B, Road::UP);

        ui->progressBar->setMaximum(targetScore);

        break;
    case TWO:

        // define win conditions
        targetScore = 2;
        remainingTime.setHMS(0,1,0);    // 1:00

        ui->stopSign1->hide();
        ui->stopSign2->show();

        // define which preset each road made in the ui should have
        ui->RoadA->setPreset(Road::A, Road::LEFT);
        ui->RoadB->setPreset(Road::A, Road::RIGHT);
        ui->Road1->setPreset(Road::DISABLED, Road::DOWN);
        ui->Road2->setPreset(Road::B, Road::UP);

        ui->progressBar->setMaximum(targetScore);

        break;
    case THREE:

        // define win conditions
        targetScore = 2;
        remainingTime.setHMS(0,1,0);    // 1:00

        ui->stopSign1->show();
        ui->stopSign2->show();

        // define which preset each road made in the ui should have
        ui->RoadA->setPreset(Road::A, Road::LEFT);
        ui->RoadB->setPreset(Road::A, Road::RIGHT);
        ui->Road1->setPreset(Road::B, Road::DOWN);
        ui->Road2->setPreset(Road::B, Road::UP);

        ui->progressBar->setMaximum(targetScore);

        break;
    }
}

void MainWindow::updateGameState()
{
    switch(state) {
    case RUN:

        // check collisions and update the ui on every frame
        // decrement the countdown every second
        connect(gameTimer, SIGNAL(timeout()), this, SLOT(checkCollisions()));
        connect(gameTimer, SIGNAL(timeout()), this, SLOT(updateUI()));
        connect(countdownTimer, SIGNAL(timeout()), this, SLOT(updateCountdown()));

        // ensure all roads know that the game is running
        ui->RoadA->setPaused(false);
        ui->RoadB->setPaused(false);
        ui->Road1->setPaused(false);
        ui->Road2->setPaused(false);

        // show ui elements
        ui->levelLabel->show();
        ui->timerLabel->show();
        ui->scoreLabel->show();
        ui->progressBar->show();

        // hide main menu elements
        ui->levelOne->hide();
        ui->levelTwo->hide();
        ui->levelThree->hide();
        ui->quit->hide();

        // hide pause menu elements
        ui->exitButton->hide();
        ui->nextButton->hide();
        ui->restartButton->hide();

        break;
    case PAUSED:

        // stop the countdown timer, make it say "paused" instead of time remaining
        ui->timerLabel->setText("Paused");
        disconnect(gameTimer, SIGNAL(timeout()), this, SLOT(updateUI()));
        disconnect(countdownTimer, SIGNAL(timeout()), this, SLOT(updateCountdown()));

        // tell the roads to stop updating
        ui->RoadA->setPaused(true);
        ui->RoadB->setPaused(true);
        ui->Road1->setPaused(true);
        ui->Road2->setPaused(true);

        // show pause menu elements
        ui->exitButton->show();
        ui->restartButton->show();

        break;
    case GAMEOVER:

        // stop the countdown timer
        ui->timerLabel->setText("Game Over");
        disconnect(gameTimer, SIGNAL(timeout()), this, SLOT(updateUI()));
        disconnect(countdownTimer, SIGNAL(timeout()), this, SLOT(updateCountdown()));

        // show game over menu elements
        ui->exitButton->show();
        ui->restartButton->show();

        break;
    case WIN:

        // stop spawning cars
        ui->RoadA->setPreset(Road::DISABLED, Road::LEFT);
        ui->RoadB->setPreset(Road::DISABLED, Road::RIGHT);
        ui->Road1->setPreset(Road::DISABLED, Road::DOWN);
        ui->Road2->setPreset(Road::DISABLED, Road::UP);

        // stop the countdown timer
        disconnect(gameTimer, SIGNAL(timeout()), this, SLOT(updateUI()));
        disconnect(countdownTimer, SIGNAL(timeout()), this, SLOT(updateCountdown()));

        // show menu elements
        ui->exitButton->show();
        // unless level 3 was just beaten
        if (level != THREE) ui->nextButton->show();

        break;
    case MENU:

        // hide ui elements
        ui->levelLabel->hide();
        ui->timerLabel->hide();
        ui->scoreLabel->hide();
        ui->progressBar->hide();

        // show main menu elements
        ui->levelOne->show();
        ui->levelTwo->show();
        ui->levelThree->show();
        ui->quit->show();

        // hide pause menu elements
        ui->exitButton->hide();
        ui->nextButton->hide();
        ui->restartButton->hide();

        // hide stop signs
        ui->stopSign1->hide();
        ui->stopSign2->hide();

        // stop spawning cars
        ui->RoadA->setPreset(Road::DISABLED, Road::LEFT);
        ui->RoadB->setPreset(Road::DISABLED, Road::RIGHT);
        ui->Road1->setPreset(Road::DISABLED, Road::DOWN);
        ui->Road2->setPreset(Road::DISABLED, Road::UP);

        break;
    }
}

void MainWindow::updateUI()
{
    // make road state match stop sign state (stop sign on -> road stopped)
    ui->Road1->toggleStop(ui->stopSign1->isClicked());
    ui->Road2->toggleStop(ui->stopSign2->isClicked());
  
    // update the score and time remaining
    ui->scoreLabel->setText("Score: " + QString::number(VerticalRoad::clearedCars) + " / " + QString::number(targetScore));
    ui->progressBar->setValue(VerticalRoad::clearedCars);
    if (VerticalRoad::clearedCars >= targetScore) {
        state = WIN;
        updateGameState();
    }
    ui->timerLabel->setText(remainingTime.toString("m:ss"));
}

void MainWindow::updateCountdown()
{
    // trigger game over if out of time
    if (remainingTime == QTime(0,0,0)) {
        state = GAMEOVER;
        updateGameState();
    }
    // remove one second from remaining time
    remainingTime = remainingTime.addSecs(-1);
}

void MainWindow::startLevelOne()
{
    // change level and update
    level = ONE;
    init();
    // change game state and update
    state = RUN;
    updateGameState();
}

void MainWindow::startLevelTwo()
{
    level = TWO;
    init();
    state = RUN;
    updateGameState();
}

void MainWindow::startLevelThree()
{
    level = THREE;
    init();
    state = RUN;
    updateGameState();
}

void MainWindow::incLevel()
{
    // don't increase past level 3
    if (level == THREE) return;
    level = (Level) ((int) level + 1);
    init();
    state = RUN;
    updateGameState();
}

void MainWindow::restart()
{
    init();
    state = RUN;
    updateGameState();
}

void MainWindow::menu()
{
    init();
    state = MENU;
    updateGameState();
}

void MainWindow::quit()
{
    QCoreApplication::quit();
}
