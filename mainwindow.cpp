#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gameTimer = new QTimer();
    gameTimer->start(33);

    connect(gameTimer, SIGNAL(timeout()), this, SLOT(checkCollisions()));
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(updateUI()));
//    connect(gameTimer, SIGNAL(timeout()), this, SLOT(updateGameState()));

    remainingTime.setHMS(0,1,0);    // 1:00
    ui->progressBar->setMaximum(10);

    countdownTimer = new QTimer();
    countdownTimer->start(1000);

    connect(countdownTimer, SIGNAL(timeout()), this, SLOT(updateCountdown()));

    // define which preset each road made in the ui should have
    ui->RoadA->setPreset(Road::DISABLED, Road::LEFT);
    ui->RoadB->setPreset(Road::H_B, Road::RIGHT);
    ui->Road1->setPreset(Road::DISABLED, Road::DOWN);
    ui->Road2->setPreset(Road::V_B, Road::UP);

    ui->backgroundWidget->init(ui->RoadA->geometry().y(), ui->RoadA->geometry().height());

    state = RUN;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space) {
        ui->Road1->toggleStop();
        ui->Road2->toggleStop();
    }

    if (event->key() == Qt::Key_Escape) {
        if (state == RUN) {
            state = PAUSED;
        } else {
            state = RUN;
        }
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
            if (car_1_bb.intersects(car_a_bb)) {
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
            if (car_1_bb.intersects(car_b_bb)) {
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
            if (car_2_bb.intersects(car_a_bb)) {
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
            if (car_2_bb.intersects(car_b_bb)) {
                car_2->notifyCollision();
                car_b->notifyCollision();
                state = GAMEOVER;
                updateGameState();
            }
        }
    }
}

void MainWindow::updateGameState() {
    switch(state) {
    case RUN:
        connect(gameTimer, SIGNAL(timeout()), this, SLOT(checkCollisions()));
        connect(gameTimer, SIGNAL(timeout()), this, SLOT(updateUI()));
        connect(countdownTimer, SIGNAL(timeout()), this, SLOT(updateCountdown()));
        ui->RoadA->setPaused(false);
        ui->RoadB->setPaused(false);
        ui->Road1->setPaused(false);
        ui->Road2->setPaused(false);
        qDebug() << "game run";
        break;
    case PAUSED:
        disconnect(gameTimer, SIGNAL(timeout()), this, SLOT(checkCollisions()));
        disconnect(gameTimer, SIGNAL(timeout()), this, SLOT(updateUI()));
        disconnect(countdownTimer, SIGNAL(timeout()), this, SLOT(updateCountdown()));
        ui->RoadA->setPaused(true);
        ui->RoadB->setPaused(true);
        ui->Road1->setPaused(true);
        ui->Road2->setPaused(true);
        qDebug() << "game pause";
        break;
    case GAMEOVER:
        disconnect(gameTimer, SIGNAL(timeout()), this, SLOT(updateUI()));
        disconnect(gameTimer, SIGNAL(timeout()), this, SLOT(checkCollisions()));
        qDebug() << "game over";
        break;
    }
}

void MainWindow::updateUI() {
    ui->scoreLabel->setText("Score: " + QString::number(VerticalRoad::clearedCars));
    ui->timerLabel->setText(remainingTime.toString("m:ss"));
    ui->progressBar->setValue(VerticalRoad::clearedCars);
}

void MainWindow::updateCountdown() {
    remainingTime = remainingTime.addSecs(-1);
}
