#ifndef ROAD_H
#define ROAD_H

#include <QOpenGLWidget>
#include <QRandomGenerator>
#include <QOpenGLFunctions>
#include <QTimer>
#include <vector>
#include <array>
#include "car.h"

class Road : public QOpenGLWidget
{
    Q_OBJECT

public:
    Road(QWidget* parent = nullptr);

    enum RoadPreset {
        H_A, H_B, V_A, V_B, DISABLED
    };

    enum Direction {
        UP, DOWN, LEFT, RIGHT
    };

    RoadPreset getPreset() const;
    void setPreset(const RoadPreset, const Direction);

    virtual void halt();

    // used like a queue to keep track of cars that are on the road
    std::vector<Car*> cars;

protected:

    QTimer* timer;

    void paintGL();
    void initializeGL();

    // right now just draws a rectangle by grabbing length and width of cars
    virtual void drawCar(Car* car);

    // helper function to construct new cars with random parameters
    virtual Car* createCar();

    virtual void spawnCar();

    RoadPreset m_preset;
    Direction m_direction;

    // hard-code the spatial gaps between cars
    // will loop over
    std::vector<qreal> m_gaps;

    int currentCar;

signals:


public slots:
    virtual void updateCars();

private:

    QOpenGLFunctions *openGLFunctions;

    void updateRelativeLoc(Car* car);

       // distance between two cars depends on movement direction
       bool carsTooClose(Car* behind, Car* front);

       // decide whether car has passed spatial gap to allow next car to spawn
       bool readyToSpawn();

};

#endif // ROAD_H
