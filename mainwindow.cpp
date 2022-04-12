#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace Tools {
    QRect getBoundingBox(Car* car) {
//        switch(car->movement) {

//        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer();
    timer->start(30);

    connect(timer, SIGNAL(timeout()), this, SLOT(checkIntersections()));

    // define which preset each road made in the ui should have
    ui->RoadA->setPreset(Road::A);
    ui->RoadB->setPreset(Road::DISABLED);
    ui->Road1->setPreset(Road::C);
    ui->Road2->setPreset(Road::DISABLED);
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

        qreal car_1_x = ui->Road1->geometry().left();
        qreal car_1_y = ((-car_1->getY()+1)/2) * ui->Road1->geometry().height();
        qreal car_1_w = ui->Road1->geometry().width();
        qreal car_1_h = (Car::l/2) * ui->Road1->geometry().height();

        // define its bounding box
        QRect car_1_bb(car_1_x, car_1_y, car_1_w, car_1_h);

        // for every car on road a
        for (Car* car_a : ui->RoadA->cars) {

            qreal car_a_x = ((car_a->getX()+1)/2) * ui->RoadA->geometry().width();
            qreal car_a_y = ui->RoadA->geometry().top();
            qreal car_a_w = (Car::l/2) * ui->RoadA->geometry().width();
            qreal car_a_h = ui->RoadA->geometry().height();

            // define its bounding box
            QRect car_a_bb(car_a_x, car_a_y, car_a_w, car_a_h);

            // check intersection between every car on road 1 and road a
            if (car_1_bb.intersects(car_a_bb)) {
                car_1->notifyCollision();
                car_a->notifyCollision();
            }
        }

        // for every car on road b
        for (Car* car_b : ui->RoadB->cars) {

            qreal car_b_x = ((car_b->getX()+1)/2) * ui->RoadB->geometry().width();
            qreal car_b_y = ui->RoadB->geometry().top();
            qreal car_b_w = (Car::l/2) * ui->RoadB->geometry().width();
            qreal car_b_h = ui->RoadB->geometry().height();

            // define its bounding box
            QRect car_b_bb(car_b_x, car_b_y, car_b_w, car_b_h);

            // check intersection between every car on road 1 and road b
            if (car_1_bb.intersects(car_b_bb)) {
                car_1->notifyCollision();
                car_b->notifyCollision();
            }
        }
    }

    // for every car on road 2
    for (Car* car_2 : ui->Road2->cars) {

        qreal car_2_x = ui->Road2->geometry().left();
        qreal car_2_y = ((-car_2->getY()+1)/2) * ui->Road2->geometry().height();
        qreal car_2_w = ui->Road2->geometry().width();
        qreal car_2_h = (Car::l/2) * ui->Road2->geometry().height();

        // define its bounding box
        QRect car_2_bb(car_2_x, car_2_y, car_2_w, car_2_h);

        // for every car on road a
        for (Car* car_a : ui->RoadA->cars) {

            qreal car_a_x = ((car_a->getX()+1)/2) * ui->RoadA->geometry().width();
            qreal car_a_y = ui->RoadA->geometry().top();
            qreal car_a_w = (Car::l/2) * ui->RoadA->geometry().width();
            qreal car_a_h = ui->RoadA->geometry().height();

            // define its bounding box
            QRect car_a_bb(car_a_x, car_a_y, car_a_w, car_a_h);

            // check intersection between every car on road 2 and road a
            if (car_2_bb.intersects(car_a_bb)) {
                car_2->notifyCollision();
                car_a->notifyCollision();
            }
        }

        // for every car on road b
        for (Car* car_b : ui->RoadB->cars) {

            qreal car_b_x = ((car_b->getX()+1)/2) * ui->RoadB->geometry().width();
            qreal car_b_y = ui->RoadB->geometry().top();
            qreal car_b_w = (Car::l/2) * ui->RoadB->geometry().width();
            qreal car_b_h = ui->RoadB->geometry().height();

            // define its bounding box
            QRect car_b_bb(car_b_x, car_b_y, car_b_w, car_b_h);

            // check intersection between every car on road 2 and road b
            if (car_2_bb.intersects(car_b_bb)) {
                car_2->notifyCollision();
                car_b->notifyCollision();
            }
        }
    }
}

