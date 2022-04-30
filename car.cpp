#include "car.h"

#include <QDebug>

Car::Car(QObject *parent)
    : QObject{parent}
{

}

qreal Car::l = 0.2;
qreal Car::w = 2;

Car::Car(MovementType movement, qreal speed) : speed(speed), movement(movement)
{
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

    color = new QColor(QRandomGenerator::global()->bounded(256),QRandomGenerator::global()->bounded(256),QRandomGenerator::global()->bounded(256));

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
}

void Car::setStopped(bool stopped) {
    this->stopped = stopped;
}

Car::Location Car::getRelativeLoc() {
    return location;
}

bool Car::isCrashed() {
    return crashed;
}

void Car::notifyCollision()
{
    crashed = true;
    QColor* red = new QColor(255,0,0);
    color = red;
}

void Car::animate()
{
    state = (!blocked && !stopped) ? DRIVING : IDLE;

    if (state == DRIVING && !crashed) {
        if (movement == RIGHT)      x += speed;
        else if (movement == LEFT)  x -= speed;
        else if (movement == UP)    y += speed;
        else if (movement == DOWN)  y -= speed;
    }
}
