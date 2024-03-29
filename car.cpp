#include "car.h"

Car::Car(QObject *parent)
    : QObject{parent}
{

}

qreal Car::l = 0.2;
qreal Car::w = 2;

Car::Car(MovementType movement, qreal speed) : speed(speed), movement(movement)
{
    // define all vertices for each car depening on its movement direction
    // these are all basically hard-coded
    switch(movement) {
    case RIGHT:
        this->vertices = { Vertex(-1 - Car::l - 0.1, 1), Vertex(-1 - Car::l - 0.11, 0.95), Vertex(-1 - Car::l - 0.12, 0.90), // back right 123
                           Vertex(-1 - Car::l - 0.12, -0.9), Vertex(-1 - Car::l - 0.11, -0.95), Vertex(-1 - Car::l - 0.1, -1), // back left 123
                           Vertex(-1 -(1./3.) * Car::l - 0.1, -1), Vertex(-1 - (1./10.)*Car::l - 0.1, -1 + (1./20.)*Car::w), Vertex(-1-0.1, -1 + (1./3.)*Car::w), // front left 123
                           Vertex(-1-0.1, 1-(1./3.)*Car::w), Vertex(-1 - (1./10.)*Car::l - 0.1, 1 - (1./20.)*Car::w), Vertex(-1 - (1./3.)*Car::l - 0.1, 1)}; // front right 123

        break;
    case LEFT:
        this->vertices = { Vertex(1 + (1./10.)*Car::l + 0.1, 1-(1./20.)*Car::w),Vertex(1+0.1, 1-(1./3.)*Car::w),
                           Vertex(1+0.1, -1+(1./3.)*Car::w), Vertex(1 + (1./10.)*Car::l + 0.1, -1 + (1./20.)*Car::w),Vertex(1 + (1./3.)*Car::l + 0.1, -1),
                           Vertex(1 + Car::l + 0.1, -1), Vertex(1 + Car::l + 0.11, -0.95), Vertex(1 + Car::l + 0.12, -0.90),
                           Vertex(1 + Car::l + 0.12, 0.9), Vertex(1 + Car::l + 0.11, 0.95), Vertex(1 + Car::l + 0.1, 1), Vertex(1+ (1./3.)*Car::l + 0.1, 1),

                         };
        break;
    case UP:

        this->vertices = {
                            Vertex(-0.95 + (1./20.)*w, -1-0.1-(1./11.)*l), //
                            Vertex(-0.90, -1-0.1-(1./6.)*l), //
                            Vertex(-1, -1-0.1-(1./3.)*l),//2.5
                            Vertex(-1,-1-0.1-(10.5/11.)*l), //
                            Vertex(-0.85, -1-0.1-l),
                            Vertex(0.85, -1-0.1-l),
                            Vertex(1,-1-0.1-(10.5/11.)*l), //
                            Vertex(1, -1-0.1-(1./3.)*l),//6.5
                            Vertex(0.90, -1-0.1-(1./6.)*l),
                            Vertex(0.95 - (1./20.)*w, -1-0.1-(1./11.)*l),
                            Vertex(1-(1./3.)*w, -1-0.1), Vertex(-1+(1./3.)*w, -1-0.1) //a
                         };
        break;
    case DOWN:

        this->vertices = {
                            Vertex(-1,1+0.1+(10.5/11.)*l), //
                            Vertex(-1, 1+0.1+(1./3.)*l), // new pt 2.5
                            Vertex(-0.9, 1+0.1+(1./6.)*l),
                            Vertex(-0.95 + (1./20.)*w, 1+0.1+(1./11.)*l),//
                            Vertex(-1+(1./3.)*w, 1+.1),Vertex(1-(1./3.)*w, 1+.1), //a
                            Vertex(0.95-(1./20.)*w, 1+.1+(1./11.)*l), //
                            Vertex(0.9, 1+.1+(1./6.)*l),
                            Vertex(1, 1+0.1+(1./3.)*l),// new point 6.5
                            Vertex(1, 1+.1+(10.5/11.)*l), //
                            Vertex(0.85, 1+.1+l), Vertex(-0.85, 1+0.1+l)
                         };
    }

    // randomly generate a color, but don't make it too dark (min RGB of 64)
    color = new QColor(QRandomGenerator::global()->bounded(64,256),QRandomGenerator::global()->bounded(64,256),QRandomGenerator::global()->bounded(64,256));

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
    // if the car is either blocked or stopped, change its state to idling
    state = (!blocked && !stopped) ? DRIVING : IDLE;

    if (state == DRIVING && !crashed) {
        if (movement == RIGHT)      offsetX += speed;
        else if (movement == LEFT)  offsetX -= speed;
        else if (movement == UP)    offsetY += speed;
        else if (movement == DOWN)  offsetY -= speed;
    }
}
