#ifndef CAR_H
#define CAR_H

#include <QObject>
#include <QTimer>

class Car : public QObject
{
    Q_OBJECT
public:
    explicit Car(QObject *parent = nullptr);
    Car(qreal speed, QColor* color);

    QColor* getColor();
    qreal getPosition();

    qreal getLength();
    qreal getWidth();

    // whether the car should be allowed to continue driving forward
    void setBlocked(bool blocked);

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

    bool shouldMove;

};

#endif // CAR_H
