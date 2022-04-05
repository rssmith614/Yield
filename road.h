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
        A, B, C
    };

    RoadPreset getPreset() const;
    void setPreset(const RoadPreset);

protected:
    void paintGL();
    void initializeGL();

signals:
    void presetChanged();


private:
    QTimer* timer;

    QOpenGLFunctions *openGLFunctions;

    // right now just draws a rectangle by grabbing length and width of cars
    // wanna make it pull a sprite
    void drawCar(Car* car);
    // helper function to construct new cars with random parameters
    Car* createCar();

    // used like a queue to keep track of cars that are on the road
    std::vector<Car*> cars;

    RoadPreset m_preset;

    // hard-code the spatial gaps between cars
    // will loop over
    std::vector<qreal> m_gaps;



    int currentCar;

};

#endif // ROAD_H
