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

public slots:
    void updateCars();
    void spawnCar();

protected:
    void drawCar(Car* car);
    virtual Car* createCar();
    virtual void paintGL();
    virtual void initializeGL();

private:
    QTimer* spawnTimer;

    QOpenGLFunctions* openGLFunctions;

    void updateRelativeLoc(Car* car);

    bool carsTooClose(Car* behind, Car* front);

    qreal distanceToIntersection(Car* car);

    bool stopped;
};

#endif // VERTICALROAD_H
