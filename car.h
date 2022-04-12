#ifndef CAR_H
#define CAR_H

#include <QObject>
#include <QTimer>
#include <QRect>
#include <QColor>

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
        BEFORE_INTERSECTION, AFTER_INTERSECTION
    };

    explicit Car(QObject *parent = nullptr);
    Car(qreal speed, QColor* color, MovementType movement);

    QColor* getColor();
    qreal getX();
    qreal getY();

    qreal getLength();
    qreal getWidth();

    MovementType getMovement();

    // block = something in front of it
    void setBlocked(bool blocked);

    // stop = stop sign in front of it
    void setStopped(bool stopped);

    void notifyCollision();

    void setLoc(Location loc);

    bool isBeforeIntersection();

    // for now every car has the same length and width
    static qreal l;
    static qreal w;

signals:
    // this is just an idea I had
//    bool blocked();

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

};

#endif // CAR_H
