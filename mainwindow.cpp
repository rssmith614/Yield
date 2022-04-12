#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer();
    timer->start(30);

    connect(timer, SIGNAL(timeout()), this, SLOT(checkIntersections()));

    // define which preset each road made in the ui should have
    ui->RoadA->setPreset(Road::B);
    ui->RoadB->setPreset(Road::A);
    ui->Road1->setPreset(Road::C);
    ui->Road2->setPreset(Road::C);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) {
        ui->Road1->toggleStop();
        ui->Road2->toggleStop();
    }
}

void MainWindow::checkIntersections() {
    // for every car on road 1
    for (Car* car_1 : ui->Road1->cars) {

        // define its bounding box
        QRect car_1_bb = Tools::getBoundingBox(car_1, ui->Road1);

        // for every car on road a
        for (Car* car_a : ui->RoadA->cars) {

            // define its bounding box
            QRect car_a_bb = Tools::getBoundingBox(car_a, ui->RoadA);

            // check intersection between every car on road 1 and road a
            if (car_1_bb.intersects(car_a_bb)) {
                car_1->notifyCollision();
                car_a->notifyCollision();
            }
        }

        // for every car on road b
        for (Car* car_b : ui->RoadB->cars) {

            // define its bounding box
            QRect car_b_bb = Tools::getBoundingBox(car_b, ui->RoadB);

            // check intersection between every car on road 1 and road b
            if (car_1_bb.intersects(car_b_bb)) {
                car_1->notifyCollision();
                car_b->notifyCollision();
            }
        }
    }

    // for every car on road 2
    for (Car* car_2 : ui->Road2->cars) {

        // define its bounding box
        QRect car_2_bb = Tools::getBoundingBox(car_2, ui->Road2);

        // for every car on road a
        for (Car* car_a : ui->RoadA->cars) {

            // define its bounding box
            QRect car_a_bb = Tools::getBoundingBox(car_a, ui->RoadA);

            // check intersection between every car on road 2 and road a
            if (car_2_bb.intersects(car_a_bb)) {
                car_2->notifyCollision();
                car_a->notifyCollision();
            }
        }

        // for every car on road b
        for (Car* car_b : ui->RoadB->cars) {

            // define its bounding box
            QRect car_b_bb = Tools::getBoundingBox(car_b, ui->RoadB);

            // check intersection between every car on road 2 and road b
            if (car_2_bb.intersects(car_b_bb)) {
                car_2->notifyCollision();
                car_b->notifyCollision();
            }
        }
    }
}

