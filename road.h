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
        A, B, C, DISABLED
    };

    RoadPreset getPreset() const;
    void setPreset(const RoadPreset);

protected:
    void paintGL();
    void initializeGL();

    // right now just draws a rectangle by grabbing length and width of cars
    virtual void drawCar(Car* car);

    // helper function to construct new cars with random parameters
    virtual Car* createCar();

    // used like a queue to keep track of cars that are on the road
    std::vector<Car*> cars;

    RoadPreset m_preset;

    // hard-code the spatial gaps between cars
    // will loop over
    std::vector<qreal> m_gaps;

    int currentCar;

signals:


public slots:
    virtual void updateCars();

private:
    QTimer* timer;

    QOpenGLFunctions *openGLFunctions;

};

#endif // ROAD_H
