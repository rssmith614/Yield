#include "car.h"

Car::Car(QObject *parent)
    : QObject{parent}
{

}

qreal Car::l = 0.25;
qreal Car::w = 2;

Car::Car(qreal speed, QColor* color) : speed(speed), color(color) {
    x = -1 - Car::l - 0.1;
    y = 0.0;

    shouldMove = true;

    timer = new QTimer(this);
    timer->start(30);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
}

QColor* Car::getColor() {
    return color;
}

qreal Car::getPosition() {
    return x;
}

void Car::setBlocked(bool blocked) {
    shouldMove = !blocked;
//    emit blocked();
}

void Car::animate() {
    // this should be more complicated...
    // probably have movement direction defined alongside speed?
    if (shouldMove) x += speed;
}
