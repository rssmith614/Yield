#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget::grabKeyboard();

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
    connect(ui->nextButton, SIGNAL (released()), this, SLOT(incLevel()));
    connect(ui->restartButton, SIGNAL(released()), this, SLOT(restart()));
    connect(ui->exitButton, SIGNAL(released()), this, SLOT(menu()));

    connect(ui->levelOne, SIGNAL(released()), this, SLOT(startLevelOne()));
    connect(ui->levelTwo, SIGNAL(released()), this, SLOT(startLevelTwo()));
    connect(ui->levelThree, SIGNAL(released()), this, SLOT(startLevelThree()));
    connect(ui->levelZen, SIGNAL(released()), this, SLOT(startLevelZen()));
    connect(ui->quit, SIGNAL(released()), this, SLOT(quit()));

    updateGameState(MENU);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // space pause button
    if (event->key() == Qt::Key_Space) {
        if (state == RUN) {
            updateGameState(PAUSED);
        } else if (state == PAUSED){
            updateGameState(RUN);
        }
    }

    // 'win' button for testing/demo
    if (event->key() == Qt::Key_D) {
        updateGameState(WIN);
    }

    // left and right arrow keys control stop signs
    if (state == RUN) {
        if (event->key() == Qt::Key_Right) {
            ui->stopSign2->toggle();
        } else if (event->key() == Qt::Key_Left) {
            ui->stopSign1->toggle();
        }
    }

    // any key to restart/advance
    if (state == GAMEOVER)
        restart();
    else if (state == WIN)
        incLevel();

}

void MainWindow::checkCollisions()
{
    // for every car on road 1
    for (Car* car_1 : ui->Road1->cars)
    {
        if (car_1->getRelativeLoc() == Car::BEFORE_INTERSECTION) continue;
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
                updateGameState(GAMEOVER);
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
                updateGameState(GAMEOVER);
            }
        }
    }

    // for every car on road 2
    for (Car* car_2 : ui->Road2->cars)
    {
        if (car_2->getRelativeLoc() == Car::BEFORE_INTERSECTION) continue;
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
                updateGameState(GAMEOVER);
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
                updateGameState(GAMEOVER);
            }
        }
    }
}

void MainWindow::init(Level level)
{
    this->level = level;
    // update on-screen level indicator
    if(level == ZEN)
    {
        ui->levelLabel->setText("Free Play");
    } else
    {
        ui->levelLabel->setText("Level " + QString::number(level));
    }

    // reset score
    VerticalRoad::clearedCars = 0;

    // remove cars from all roads
    ui->Road1->clear();
    ui->Road2->clear();
    ui->RoadA->clear();
    ui->RoadB->clear();

    switch(level) {
    case ZEN:
        targetScore = INT_MAX;
        remainingTime.setHMS(1,0,0);    // 1:00

        ui->stopSign1->show();
        ui->stopSign2->show();
        ui->stopSign1->set(true);
        ui->stopSign2->set(true);

        ui->progressBar->hide();

        // define which preset each road made in the ui should have
        ui->RoadA->setPreset(Road::RAND_C, Road::LEFT);
        ui->RoadB->setPreset(Road::RAND_C, Road::RIGHT);
        ui->Road1->setPreset(Road::FIXED, Road::DOWN);
        ui->Road2->setPreset(Road::FIXED, Road::UP);

        ui->progressBar->setMaximum(targetScore);

        break;


    case ONE:

        // define win conditions
        targetScore = 10;
        remainingTime.setHMS(0,1,0);    // 1:00

        // only road 2 is active, so hide the other stop sign
        ui->stopSign1->hide();
        ui->stopSign2->show();
        ui->stopSign1->set(true);
        ui->stopSign2->set(true);
        // define which preset each road made in the ui should have
        ui->RoadA->setPreset(Road::DISABLED, Road::LEFT);
        ui->RoadB->setPreset(Road::RAND_B, Road::RIGHT);
        ui->Road1->setPreset(Road::DISABLED, Road::DOWN);
        ui->Road2->setPreset(Road::RAND_U, Road::UP);

        ui->progressBar->setMaximum(targetScore);

        break;
    case TWO:

        // define win conditions
        targetScore = 15;
        remainingTime.setHMS(0,1,0);    // 1:00

        ui->stopSign1->hide();
        ui->stopSign2->show();
        ui->stopSign1->set(true);
        ui->stopSign2->set(true);

        // define which preset each road made in the ui should have
        ui->RoadA->setPreset(Road::RAND_A, Road::LEFT);
        ui->RoadB->setPreset(Road::RAND_C, Road::RIGHT);
        ui->Road1->setPreset(Road::DISABLED, Road::DOWN);
        ui->Road2->setPreset(Road::RAND_U, Road::UP);

        ui->progressBar->setMaximum(targetScore);

        break;
    case THREE:

        // define win conditions
        targetScore = 20;
        remainingTime.setHMS(0,1,0);    // 1:00

        ui->stopSign1->show();
        ui->stopSign2->show();
        ui->stopSign1->set(true);
        ui->stopSign2->set(true);

        // define which preset each road made in the ui should have
        ui->RoadA->setPreset(Road::RAND_C, Road::LEFT);
        ui->RoadB->setPreset(Road::RAND_A, Road::RIGHT);
        ui->Road1->setPreset(Road::RAND_U, Road::DOWN);
        ui->Road2->setPreset(Road::RAND_U, Road::UP);

        ui->progressBar->setMaximum(targetScore);

        break;
    }
}

void MainWindow::updateGameState(GameState state)
{
    this->state = state;
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

        ui->stopSign1->setEnabled(true);
        ui->stopSign2->setEnabled(true);

        // show ui elements
        ui->levelLabel->show();
        ui->timerLabel->show();
        ui->scoreLabel->show();

        if(level != ZEN)
        {
            ui->progressBar->show();
        }


        // hide main menu elements
        ui->title->hide();
        ui->levelOne->hide();
        ui->levelTwo->hide();
        ui->levelThree->hide();
        ui->levelZen->hide();
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

        ui->stopSign1->setEnabled(false);
        ui->stopSign2->setEnabled(false);

        // show pause menu elements
        ui->title->show();
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

        ui->stopSign1->setEnabled(false);
        ui->stopSign2->setEnabled(false);

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
        disconnect(gameTimer, SIGNAL(timeout()), this, SLOT(checkCollisions()));

        // show menu elements
        ui->exitButton->show();
        // unless level 3 was just beaten
        if (level != THREE) ui->nextButton->show();

        ui->stopSign1->setEnabled(false);
        ui->stopSign2->setEnabled(false);

        break;
    case MENU:

        // hide ui elements
        ui->levelLabel->hide();
        ui->timerLabel->hide();
        ui->scoreLabel->hide();
        ui->progressBar->hide();

        // show main menu elements
        ui->title->show();
        ui->levelOne->show();
        ui->levelTwo->show();
        ui->levelThree->show();
        ui->levelZen->show();
        ui->quit->show();

        // hide pause menu elements
        ui->exitButton->hide();
        ui->nextButton->hide();
        ui->restartButton->hide();

        // hide stop signs
        ui->stopSign1->hide();
        ui->stopSign2->hide();
        ui->stopSign1->setEnabled(false);
        ui->stopSign2->setEnabled(false);

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
    if(level == ZEN)
    {
        ui->scoreLabel->setText("Score: " + QString::number(VerticalRoad::clearedCars));
        Road::speed = 0.015 + qLn(VerticalRoad::clearedCars+1)*0.001;
    } else
    {
        ui->scoreLabel->setText("Score: " + QString::number(VerticalRoad::clearedCars) + " / " + QString::number(targetScore));
    }

    ui->progressBar->setValue(VerticalRoad::clearedCars);
    if (VerticalRoad::clearedCars >= targetScore) {
        updateGameState(WIN);
    }
    ui->timerLabel->setText(remainingTime.toString("m:ss"));
}

void MainWindow::updateCountdown()
{
    // trigger game over if out of time
    if (remainingTime == QTime(0,0,0)) {
        updateGameState(GAMEOVER);
    }
    // remove one second from remaining time
    if(level == ZEN)
    {
        remainingTime = remainingTime.addSecs(1);
    } else
    {
        remainingTime = remainingTime.addSecs(-1);
    }
}

void MainWindow::startLevelOne()
{
    // change level and update
    init(ONE);
    // change game state
    updateGameState(RUN);
}

void MainWindow::startLevelTwo()
{
    init(TWO);
    updateGameState(RUN);
}

void MainWindow::startLevelThree()
{
    init(THREE);
    updateGameState(RUN);
}

void MainWindow::startLevelZen()
{
    level = ZEN;
    init();
    state = RUN;
    updateGameState();
}

void MainWindow::incLevel()
{
    // don't increase past level 3
    if (level == THREE) return;
    init((Level) ((int) level + 1));
    updateGameState(RUN);
}

void MainWindow::restart()
{
    init(level);
    updateGameState(RUN);
}

void MainWindow::menu()
{
    init(level);
    updateGameState(MENU);
}

void MainWindow::quit()
{
    QCoreApplication::quit();
}
