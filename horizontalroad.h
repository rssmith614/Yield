#ifndef HORIZONTALROAD_H
#define HORIZONTALROAD_H

#include "road.h"

class HorizontalRoad : public Road
{
    Q_OBJECT
public:
    HorizontalRoad(QWidget* parent);

protected:
    Car* createCar();

    void updateRelativeLoc(Car* car);

    bool carsTooClose(Car* behind, Car* front);

protected slots:
    void updateCars();
};

#endif // HORIZONTALROAD_H
