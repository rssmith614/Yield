#ifndef TOOLS_H
#define TOOLS_H

#include <QRect>
#include <QOpenGLWidget>

#include "car.h"

namespace Tools {
    // convert the car from its relative size/position (-1 to 1 on the road) to a
    // global position based on window size and pixel count
    QRect getBoundingBox(Car* car, QOpenGLWidget* road) {
        // x, y = top left corner of car
        // w, h move down and right relative to corner
        qreal car_x, car_y, car_w, car_h;
        switch(car->getMovement()) {
        case Car::RIGHT:
        case Car::LEFT:
            // map x-pos from -1 to 1 -> 0 to 600
            car_x = ((car->getX()+1)/2) * road->geometry().width();
            // since car is the width of the road, this works
            car_y = road->geometry().top();
            // Car::l is a proportion of road size with scale -1 to 1, so we need to make it a proportion of
            // the length of the whole road, then multiply by the length of the road
            car_w = (Car::l/2 - (Car::l/20)) * road->geometry().width();
            // since car is the width of the road, this works
            car_h = road->geometry().height();
            break;
        case Car::UP:
        case Car::DOWN:
            // car is the width of the road so this works
            car_x = road->geometry().left();
            // map y-pos from -1 to 1 -> 600 to 0
            // NOTE: this is backwards from right-left because while -1 and 600 correspond to the bottom
            // here, -1 and 0 correspond to the left side for the horizontal case. Just negate the car's y-pos to fix this
            car_y = ((-car->getY()+1)/2) * road->geometry().height();
            // car is the width of the road
            car_w = road->geometry().width();
            // proportion of the road's length
            car_h = (Car::l/2 - (Car::l/18)) * road->geometry().height();
            break;
        }

        QRect bb(car_x, car_y, car_w, car_h);
        return bb;
    }
}

#endif // TOOLS_H
