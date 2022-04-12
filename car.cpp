#include "car.h"

#include <QDebug>

Car::Car(QObject *parent)
    : QObject{parent}
{

}

qreal Car::l = 0.2;
qreal Car::w = 2;

Car::Car(qreal speed, QColor* color, MovementType movement) : speed(speed), color(color), movement(movement) {
    switch(movement) {
    case RIGHT:
        x = -1 - Car::l - 0.1;
        y = 0.0;
        break;
    case LEFT:
        x = 1 + 0.1;
        y = 0.0;
        break;
    case UP:
        x = 0.0;
        y = -1 - 0.1;
        break;
    case DOWN:
        x = 0.0;
        y = 1 + Car::l + 0.1;
    }

    stopped = false;
    blocked = false;

    crashed = false;

    state = DRIVING;
    location = BEFORE_INTERSECTION;

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

Car::MovementType Car::getMovement() {
    return movement;
}

void Car::setLoc(Location loc) {
    location = loc;
}

void Car::setBlocked(bool blocked) {
    this->blocked = blocked;
//    state = (blocked) ? STOPPED : DRIVING;
}

void Car::setStopped(bool stopped) {
    this->stopped = stopped;
}

void Car::notifyCollision() {
    crashed = true;
    QColor* red = new QColor(255,0,0);
    color = red;
}

bool Car::isBeforeIntersection() {
    return location == BEFORE_INTERSECTION;
}

void Car::animate() {
    state = (!blocked && !stopped) ? DRIVING : IDLE;

    if (state == DRIVING && !crashed) {
        if (movement == RIGHT)      x += speed;
        else if (movement == LEFT)  x -= speed;
        else if (movement == UP)    y += speed;
        else if (movement == DOWN)  y -= speed;
    }
}
