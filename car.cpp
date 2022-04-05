#include "car.h"

Car::Car(QObject *parent)
    : QObject{parent}
{

}

qreal Car::l = 0.25;
qreal Car::w = 2;

Car::Car(qreal speed, QColor* color, MovementType movement) : speed(speed), color(color), movement(movement) {
    switch(movement) {
    case LtoR:
        x = -1 - Car::l - 0.1;
        y = 0.0;
        break;
    case RtoL:
        x = 1 + Car::l + 0.1;
        y = 0.0;
        break;
    case BtoT:
        x = 0.0;
        y = -1 - Car::l - 0.1;
    }

    shouldMove = true;

    timer = new QTimer(this);
    timer->start(30);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
}

QColor* Car::getColor() {
    return color;
}

qreal Car::getX() {
    return x;
}

qreal Car::getY() {
    return y;
}

void Car::setBlocked(bool blocked) {
    shouldMove = !blocked;
//    emit blocked();
}

void Car::animate() {
    // this should be more complicated...
    // probably have movement direction defined alongside speed?
    if (shouldMove) {
        if (movement == LtoR) x += speed;
        else if (movement == RtoL) x -= speed;
        else if (movement == BtoT) y += speed;
    }
}
