#ifndef VERTICALROAD_H
#define VERTICALROAD_H

#include "road.h"
#include <QObject>
#include <QOpenGLWidget>

class VerticalRoad : public Road
{
    Q_OBJECT
public:
    VerticalRoad(QWidget* parent = nullptr);

//    bool stopped();
    void toggleStop();

    qreal intersectionLoc;

public slots:
    void updateCars();

protected:
    virtual void drawCar(Car* car);
    virtual Car* createCar();
    virtual void paintGL();
    virtual void initializeGL();

private:
    QOpenGLFunctions* openGLFunctions;

    bool stopped;
};

#endif // VERTICALROAD_H
