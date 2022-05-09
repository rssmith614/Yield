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
        DISABLED = 0, RAND_A, RAND_B, RAND_C, FIXED
    };

    enum Direction {
        UP, DOWN, LEFT, RIGHT
    };

    RoadPreset getPreset() const;
    void setPreset(const RoadPreset, const Direction);

    void setPaused(bool paused);

    // used like a queue to keep track of cars that are on the road
    std::vector<Car*> cars;

    void clear();
    static float speed;

protected:

    QTimer* timer;

    // for temporal spawn gaps
    QTimer* spawnTimer;

    void paintGL();
    void initializeGL();

    // right now just draws a rectangle by grabbing length and width of cars
    void drawCar(Car* car);

    // helper function to construct new cars with random parameters
    virtual Car* createCar();

    RoadPreset preset;
    Direction direction;

    // hard-code the spatial gaps between cars
    // will loop over
    std::vector<qreal> gaps;

    int currentCar;

    bool paused;

protected slots:
    virtual void updateCars();
    // construct a car and add it to the queue
    virtual void spawnCar();

private:

    QOpenGLFunctions *openGLFunctions;

    // before/after intersection and off-screen depend on the car's direction
    void updateRelativeLoc(Car* car);

    // distance between two cars depends on movement direction
    bool carsTooClose(Car* behind, Car* front);

    // decide whether car has passed spatial gap to allow next car to spawn
    bool readyToSpawn();

    // decide whether the preset corresponds to the road direction
    bool validPreset();

};

#endif // ROAD_H
