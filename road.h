#ifndef ROAD_H
#define ROAD_H

#include <QOpenGLWidget>
#include <QRandomGenerator>
#include <QOpenGLFunctions>
#include <QTimer>
#include <vector>
#include "car.h"

class Road : public QOpenGLWidget
{
    Q_OBJECT

public:
    Road(QWidget* parent = nullptr);

    enum RoadPreset {
        DISABLED = 0, RAND_A, RAND_B, RAND_C, RAND_U
    };

    enum Direction {
        UP, DOWN, LEFT, RIGHT
    };

    void setPreset(const RoadPreset, const Direction);

    void setPaused(bool paused);

    // used like a queue to keep track of cars that are on the road
    std::vector<Car*> cars;

    void clear();

protected:

    QTimer* timer;

    // for temporal spawn gaps
    QTimer* spawnTimer;

    void paintGL();
    void initializeGL();

    void drawCar(Car* car);

    RoadPreset preset;
    Direction direction;

    std::vector<qreal> gaps;

    int currentCar;

    bool paused;

    // helper function to construct new cars with random parameters
    virtual Car* createCar() = 0;

    // before/after intersection and off-screen depend on the car's direction
    virtual void updateRelativeLoc(Car*) = 0;

    // distance between two cars depends on movement direction
    virtual bool carsTooClose(Car*, Car*) = 0;

protected slots:
    virtual void updateCars() = 0;
    // construct a car and add it to the queue
    void spawnCar();

private:

    QOpenGLFunctions *openGLFunctions;
};

#endif // ROAD_H
