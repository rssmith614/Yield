#ifndef CAR_H
#define CAR_H

#include <QObject>
#include <QTimer>
#include <QRect>
#include <QColor>
#include <QRandomGenerator>

class Car : public QObject
{
    Q_OBJECT
public:
    enum MovementType {
        RIGHT, LEFT, UP, DOWN
    };

    enum State {
        DRIVING, IDLE
    };

    enum Location {
        BEFORE_INTERSECTION, AFTER_INTERSECTION, OFF_SCREEN
    };

    explicit Car(QObject *parent = nullptr);
    Car(MovementType movement, qreal speed = 0.01);

    QColor* getColor();
    qreal getX();
    qreal getY();

    MovementType getMovement();

    // block = something in front of it
    void setBlocked(bool blocked);

    // stop = stop sign in front of it
    void setStopped(bool stopped);

    // stop and turn red
    void notifyCollision();

    // update relative location - before/after intersection, off screen
    void setLoc(Location loc);

    // get relative location - before/after intersection, off screen
    Location getRelativeLoc();

    bool isCrashed();

    // for now every car has the same length and width
    static qreal l;
    static qreal w;

protected slots:
    // intended to be connected to QTimer timeout() signal (each frame)
    // define how the car moves on each frame
    void animate();

private:
    QTimer* timer;

    qreal speed;
    QColor* color;
    qreal x;
    qreal y;

    MovementType movement;
    State state;
    Location location;

    bool stopped;
    bool blocked;

    bool crashed;

};

#endif // CAR_H
