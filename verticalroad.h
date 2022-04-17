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

    void toggleStop();

    qreal intersectionLoc;

    static int clearedCars;

public slots:
    void updateCars();
    void spawnCar();

protected:
    virtual void drawCar(Car* car);
    virtual Car* createCar();
    virtual void paintGL();
    virtual void initializeGL();

private:
    // for temporal spawn gaps
    QTimer* spawnTimer;

    QOpenGLFunctions* openGLFunctions;

    // before/after intersection and off-screen depend on the car's direction
    void updateRelativeLoc(Car* car);

    // distance between two cars depends on movement direction
    bool carsTooClose(Car* behind, Car* front);

    // relative distance to intersection depends on the car's direction
    qreal distanceToIntersection(Car* car);

    bool stopped;
};

#endif // VERTICALROAD_H
