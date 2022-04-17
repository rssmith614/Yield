#include "car.h"

#include <QDebug>

Car::Car(QObject *parent)
    : QObject{parent}
{

}

qreal Car::l = 0.2;
qreal Car::w = 2;

Car::Car(qreal speed, MovementType movement) : speed(speed), movement(movement) {
    switch(movement) {
    case RIGHT:
//        x = -1 - Car::l - 0.1;
//        y = 0.0;
        this->vertices = { Vertex(-1 - Car::l - 0.1, 1), Vertex(-1 - Car::l - 0.1, -1), Vertex(-1 -(1./3.) * Car::l - 0.1, -1),
                           Vertex(-1-0.1, -1 + (1./7.)*Car::w), Vertex(-1-0.1, 1-(1./7.)*w), Vertex(-1 - (1./3.)*Car::l - 0.1, 1)};

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

    offsetX = 0;
    offsetY = 0;
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
    return vertices[0].x + offsetX;
}

qreal Car::getY() {
    return vertices[0].y + offsetY;
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

Car::Location Car::getRelativeLoc() {
    return location;
}

void Car::animate() {
//    qDebug() << "animating car" << this;
    state = (!blocked && !stopped) ? DRIVING : IDLE;

    if (state == DRIVING && !crashed) {
        if (movement == RIGHT)      offsetX += speed;
        else if (movement == LEFT)  offsetX -= speed;
        else if (movement == UP)    offsetY += speed;
        else if (movement == DOWN)  offsetY -= speed;
    }
}
